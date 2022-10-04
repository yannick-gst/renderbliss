// Copyright (c) 2008-2011 Yannick Tapsoba.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "renderbliss/Integrators/PhotonIntegrator.h"
#include <algorithm>
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <ctime>
#include "renderbliss/Scene.h"
#include "renderbliss/Integrators/DirectIlluminationUtils.h"
#include "renderbliss/Integrators/PhotonMapping/PhotonMap.h"
#include "renderbliss/Interfaces/IJob.h"
#include "renderbliss/Interfaces/ILight.h"
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Lights/Luminaire.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Sampling/SamplingFunctions.h"
#include "renderbliss/Utils/Utils.h"
#include "renderbliss/Utils/JobScheduler.h"
#include "renderbliss/Utils/StatsTracker.h"

namespace renderbliss
{
// Helper class to generate a scrambled Halton sequence as to minimize
// correlation problems in higher dimensions and in a concurrent usage
class PermutedHalton
{
public:

    PermutedHalton(uint32 numDimensions, MersenneTwister& rng);
    template <unsigned NumSampledDimensions>
    void NthSample(uint32 n, boost::array<real, NumSampledDimensions>& sample) const;

private:

    std::vector<uint32> bases;
    std::vector<uint32> permutations;
    uint32 numDimensions;
    mutable MersenneTwister& rng;
};

// Job class for photon shooting parallelization
class PhotonShootingJob : public IJob
{
public:

    PhotonShootingJob(uint32 maxPhotonDepth, const Scene& scene, const StepFunctionSampler* powerDistribution,
                      StatsTracker& stats, IrradiancePhotonMap& sharedIndirectMap, CausticPhotonMap& sharedCausticMap,
                      DirectPhotonMap& sharedDirectMap, boost::mutex& sharedMutex);
    virtual void Run() const;

private:

    mutable MersenneTwister rng;
    PermutedHalton halton;

    // Local photon data for delayed storage into the shared photon maps
    mutable struct
    {
        std::vector<Vector3> directions;
        std::vector<Vector3> positions;
        std::vector<Spectrum> powers;
        void Clear() { directions.clear(), positions.clear(), powers.clear(); };
    } causticPhotons, directPhotons;
    mutable struct
    {
        std::vector<Vector3> directions;
        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Spectrum> powers;
        void Clear() { directions.clear(), positions.clear(), normals.clear(), powers.clear(); };
    } indirectPhotons;

    // Maximum number of bounces per photon
    const uint32 maxPhotonDepth;

    // Scene in which the photons will be emitted
    const Scene& scene;

    // Lighting power distribution
    const StepFunctionSampler* powerDistribution;

    // Shared members will be written concurrently
    mutable StatsTracker& stats;
    mutable IrradiancePhotonMap& sharedIndirectMap;
    mutable CausticPhotonMap& sharedCausticMap;
    mutable DirectPhotonMap& sharedDirectMap;
    mutable boost::mutex& sharedMutex;

    // Track whether the shared maps are full
    mutable bool causticDone;
    mutable bool indirectDone;
};
}

namespace renderbliss
{
PermutedHalton::PermutedHalton(uint32 numDimensions, MersenneTwister& rng)
    : numDimensions(numDimensions), rng(rng)
{
    // Initialize bases
    uint32 sumBases = 0;
    bases.reserve(this->numDimensions);
    for (uint32 i = 0; i < numDimensions; ++i)
    {
        uint32 prime = Prime(i);
        bases.push_back(prime);
        sumBases += prime;
    }

    // Generate one permutation per dimension
    permutations.reserve(sumBases);
    uint32 permStart = 0;
    for (uint32 i = 0; i < numDimensions; ++i)
    {
        for (uint32 j = 0; j < bases[i]; ++j)
        {
            permutations.push_back(j);
        }
        Shuffle(rng, &permutations[permStart], bases[i]);
        permStart += bases[i];
    }
}

template <unsigned NumSampledDimensions>
void PermutedHalton::NthSample(uint32 n, boost::array<real, NumSampledDimensions>& sample) const
{
    RB_ASSERT(NumSampledDimensions <= numDimensions);
    const uint32* p = &permutations[0];
    for (uint32 i = 0; i < numDimensions; ++i)
    {
        sample[i] = std::min(PermutedRadicalInverse(n, bases[i], p), 1.0f-Epsilon());
        p+=bases[i];
    }
}

PhotonShootingJob::PhotonShootingJob(uint32 maxPhotonDepth, const Scene& scene, const StepFunctionSampler* powerDistribution,
                                     StatsTracker& stats, IrradiancePhotonMap& sharedIndirectMap, CausticPhotonMap& sharedCausticMap,
                                     DirectPhotonMap& sharedDirectMap, boost::mutex& sharedMutex)
    : rng(static_cast<uint>(rand())), halton(6,rng), maxPhotonDepth(maxPhotonDepth),
      scene(scene), powerDistribution(powerDistribution), stats(stats),
      sharedIndirectMap(sharedIndirectMap), sharedCausticMap(sharedCausticMap),
      sharedDirectMap(sharedDirectMap), sharedMutex(sharedMutex),
      causticDone(sharedCausticMap.Full()), indirectDone(sharedIndirectMap.Full())
{
}

void PhotonShootingJob::Run() const
{
    const LightPtrList& lights = scene.Lights();
    if (lights.empty()) return;

    // Get photon tracing statistics trackers
    AtomicCounter& causticPaths = stats.Counter("Photon Tracing", "Caustic paths");
    AtomicCounter& directPaths = stats.Counter("Photon Tracing", "Direct paths");
    AtomicCounter& emittedPhotons = stats.Counter("Photon Tracing", "Emitted photons");
    AtomicCounter& indirectPaths = stats.Counter("Photon Tracing", "Indirect paths");
    AtomicCounter& storedCausticPhotons = stats.Counter("Photon Tracing", "Stored caustic photons");
    AtomicCounter& storedDirectPhotons = stats.Counter("Photon Tracing", "Stored direct photons");
    AtomicCounter& storedIndirectPhotons = stats.Counter("Photon Tracing", "Stored global photons");

    uint32 sampleNo = 0;

    while (!(causticDone && indirectDone))
    {
        const uint32 blockSize = 512; // Count of photons to emit at a time
        for (uint32 i = 0; i < blockSize; ++i)
        {
            boost::array<real, 6> sample;
            halton.NthSample(++sampleNo, sample);

            // Pick the light to sample
            size_t lightPick = powerDistribution ? powerDistribution->SampleIndex(sample[0]) : rng.RandomUint(lights.size()-1);
            real lightPdf = powerDistribution ? powerDistribution->FunctionValue(lightPick)/powerDistribution->FunctionIntegral() : 1.0f/lights.size();

            // Generate the photon ray
            real photonRayPdf = 0.0f;
            Vector3 lightNormal;
            Vector3 photonRayOrigin;
            Vector3 photonRayDirection;
            const LightConstPtr& lightToSample = lights[lightPick];
            lightToSample->SampleOutgoingRay(sample.data()+1, photonRayOrigin, photonRayDirection, lightNormal, photonRayPdf);
            if (photonRayPdf <= 0.0f) continue;

            // Proceed to photon tracing
            Spectrum photonPower;
            Intersection photonHit;
            bool isPathSpecular = true; // We assume that the path is specular at the beginning for code simplicity
            Ray photonRay(photonRayOrigin, photonRayDirection);
            uint32 numBounces = 0;
            while (scene.Intersects(photonRay, photonHit))
            {
                RB_ASSERT(photonHit.material);
                ++numBounces;
                if (numBounces == 1) // The photon is just leaving the light, initialize its power
                {
                    photonPower = lightToSample->EmittedRadiance(lightNormal, photonRayDirection);
                    if (photonPower.IsBlack()) break;
                    photonPower *= AbsDotProduct(lightNormal, photonRayDirection);
                    photonPower /= (lightPdf * photonRayPdf);
                    if (!indirectDone)
                    {
                        directPhotons.directions.push_back(-photonRay.Direction().GetNormalized());
                        directPhotons.positions.push_back(photonHit.point);
                        directPhotons.powers.push_back(photonPower);
                    }
                }

                Vector3 unitPhotonDir = photonRay.Direction().GetNormalized();

                // Store photon after first bounce if surface hit has non-specular components
                if ((numBounces > 1) && photonHit.material->MatchesFlags(~BsdfCombinedFlags::Delta))
                {
                    // If the photon has only specularly bounced so far, we must store it
                    // in the caustic map. Otherwise, we must store it in the global map.
                    if (isPathSpecular)
                    {
                        if (!causticDone)
                        {
                            causticPhotons.directions.push_back(-unitPhotonDir);
                            causticPhotons.positions.push_back(photonHit.point);
                            causticPhotons.powers.push_back(photonPower);
                        }
                    }
                    else if (!indirectDone)
                    {
                        indirectPhotons.directions.push_back(-unitPhotonDir);
                        indirectPhotons.positions.push_back(photonHit.point);
                        indirectPhotons.normals.push_back(photonHit.uvn.N());
                        indirectPhotons.powers.push_back(photonPower);
                    }
                }
                if (numBounces >= maxPhotonDepth) break;
                // Sample a new photon direction
                BsdfSamplingRecord brec(photonHit, rng, BsdfCombinedFlags::All);
                photonHit.material->SampleBsdf(-unitPhotonDir, brec);
                if ((brec.pdf <= 0.0f) || (brec.value.IsBlack())) break;

                // Use Russian roulette to decide whether to bounce or absorb the photon
                Spectrum newPower = photonPower * brec.value * AbsDotProduct(brec.sampledDirection, photonHit.uvn.N()) / brec.pdf;
                float bouncingProbability = std::min(1.0f, newPower.Luminance()/photonPower.Luminance());
                if (rng.CanonicalRandom() > bouncingProbability) break;
                photonPower = newPower/bouncingProbability;
                isPathSpecular &= photonHit.material->MatchesFlags(brec.sampledComponentIndex, BsdfCombinedFlags::Delta); // Update path "specularity"
                if (indirectDone && !isPathSpecular) break;
                photonRay = Ray(photonHit.point, brec.sampledDirection);
            }
        }

        emittedPhotons.Add(blockSize);

        // Update the shared global and caustic photon maps
        {
            boost::mutex::scoped_lock lock(sharedMutex);
            if (!indirectDone)
            {
                sharedIndirectMap.StorePhotons(indirectPhotons.positions, indirectPhotons.directions, indirectPhotons.normals, indirectPhotons.powers);
                indirectDone = sharedIndirectMap.Full();
                indirectPaths.Add(blockSize);
                storedIndirectPhotons.Add(static_cast<uint32>(indirectPhotons.positions.size()));

                sharedDirectMap.StorePhotons(directPhotons.positions, directPhotons.directions, directPhotons.powers);
                directPaths.Add(blockSize);
                storedDirectPhotons.Add(static_cast<uint32>(directPhotons.positions.size()));
            }
            if (!causticDone)
            {
                sharedCausticMap.StorePhotons(causticPhotons.positions, causticPhotons.directions, causticPhotons.powers);
                causticDone = sharedCausticMap.Full();
                causticPaths.Add(blockSize);
                storedCausticPhotons.Add(static_cast<uint32>(causticPhotons.positions.size()));
            }
        }
        causticPhotons.Clear();
        directPhotons.Clear();
        indirectPhotons.Clear();
    }
}

PhotonIntegrator::Settings::Settings(const PropertyMap& props)
{
    props.Get<PropertyMap>("global_photon_map", PropertyMap(), gpmProps);
    props.Get<PropertyMap>("caustic_photon_map", PropertyMap(), cpmProps);
    props.Get<uint32>("specular_depth", 4, specularDepth);
    props.Get<uint32>("shadow_rays", 4, numShadowRays);
    props.Get<uint32>("photon_depth", 4, maxPhotonDepth);
    props.Get<uint32>("final_gathering_samples", 16, numFinalGatheringSamples);
}

PhotonIntegrator::PhotonIntegrator(const PropertyMap& props, StatsTracker& stats)
    : SurfaceIntegrator(stats), settings(props)
{
    causticPhotonMap.reset(new CausticPhotonMap(settings.cpmProps));
    indirectPhotonMap.reset(new IrradiancePhotonMap(settings.gpmProps));
    stats.AddCounter("Intersections", "Intersection tests");
    stats.AddCounter("Intersections", "Intersection hits");
    stats.AddCounter("Photon Tracing", "Caustic paths");
    stats.AddCounter("Photon Tracing", "Direct paths");
    stats.AddCounter("Photon Tracing", "Emitted photons");
    stats.AddCounter("Photon Tracing", "Indirect paths");
    stats.AddCounter("Photon Tracing", "Stored caustic photons");
    stats.AddCounter("Photon Tracing", "Stored direct photons");
    stats.AddCounter("Photon Tracing", "Stored indirect photons");
    stats.AddCounter("Rays", "Final gathering rays traced");
    stats.AddCounter("Rays", "Primary rays traced");
    stats.AddCounter("Rays", "Secondary rays traced");
    stats.AddCounter("Rays", "Shadow rays traced");
}

Spectrum PhotonIntegrator::FinalGathering(const Scene& scene, const Intersection& hit, const Vector3& outgoing, MersenneTwister& rng) const
{
    if (!hit.material) { return Spectrum::black; }
    if (!settings.numFinalGatheringSamples) { return indirectPhotonMap->RadianceEstimate(hit, outgoing); }

    uint32 numGathered = 0;
    size_t sampleIndex = 0;
    std::vector<real> samples;
    Spectrum result = Spectrum::black;

    GenerateLatinHypercubeSamples(rng, samples, settings.numFinalGatheringSamples, 3);
    AtomicCounter& numFinalGatheringRays = stats.Counter("Rays", "Final gathering rays traced");

    for (uint32 i = 0; i < settings.numFinalGatheringSamples; ++i)
    {
        BsdfSamplingRecord brec(hit, &samples[sampleIndex+3*i], BsdfCombinedFlags::Diffuse|BsdfCombinedFlags::Glossy);
        hit.material->SampleBsdf(outgoing, brec);
        if ((brec.pdf==0.0f) || brec.value.IsBlack()) continue;

        Intersection finalGatheringHit;
        Ray finalGatheringRay(hit.point, brec.sampledDirection);
        if (scene.Intersects(finalGatheringRay, finalGatheringHit))
        {
            Spectrum indirectIllumination = indirectPhotonMap->RadianceEstimate(finalGatheringHit, -finalGatheringRay.Direction());
            real cosFactor = AbsDotProduct(hit.uvn.N(), finalGatheringRay.Direction());
            result += brec.value * indirectIllumination * cosFactor / brec.pdf;
            ++numGathered;
        }
        ++numFinalGatheringRays;
    }

    if (numGathered)
    {
        result /= static_cast<float>(numGathered);
        return result;
    }

    return indirectPhotonMap->RadianceEstimate(hit, outgoing);
}

void PhotonIntegrator::PreProcess(const Scene& scene, JobScheduler& scheduler)
{
    powerDistribution.reset(PowerDistribution(scene).release());
    boost::shared_ptr<DirectPhotonMap> directPhotonMap(new DirectPhotonMap(settings.gpmProps));
    // Schedule photon shooting jobs
    boost::mutex mutex;
    unsigned numThreads = HardwareThreadCount();
    for (unsigned i = 0; i < numThreads; ++i)
    {
        JobConstPtr job(new PhotonShootingJob(settings.maxPhotonDepth, scene, powerDistribution.get(), stats,
                                              *indirectPhotonMap, *causticPhotonMap, *directPhotonMap, mutex));
        scheduler.Spawn(job);
    }
    scheduler.WaitForAllJobs();

    const uint32 numCausticPaths = stats.Counter("Photon Tracing", "Caustic paths");
    causticPhotonMap->ScalePower(1.0f/numCausticPaths);
    causticPhotonMap->Balance();

    const uint32 numDirectPaths = stats.Counter("Photon Tracing", "Direct paths");
    directPhotonMap->ScalePower(1.0f/numDirectPaths);
    directPhotonMap->Balance();

    const uint32 numIndirectPaths = stats.Counter("Photon Tracing", "Indirect paths");
    indirectPhotonMap->ScalePower(1.0f/numIndirectPaths);
    indirectPhotonMap->Balance();
    indirectPhotonMap->PrecomputeIrradianceEstimate(*causticPhotonMap.get(), *directPhotonMap.get());
}

Spectrum PhotonIntegrator::Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const
{
    Spectrum L(Spectrum::black);

    Intersection hit;

    if (ray.depth == 0)
    {
        ++stats.Counter("Rays", "Primary rays traced");
    }

    if (!scene.Intersects(ray, hit))
    {
        return L;
    }

    Vector3 toViewer = -ray.Direction().GetNormalized();

    if (hit.emitter && (ray.depth==0))
    {
        L += hit.emitter->EmittedRadiance(hit.uvn.N(), toViewer);
    }

    // Add direct illumination
    L += DirectIllumination(scene, toViewer, hit, powerDistribution.get(), settings.numShadowRays, rng, opacity, stats.Counter("Rays", "Shadow rays traced"));

    // Add indirect illumination using one-bounce final gathering
    if (!indirectPhotonMap->Empty())
    {
        L += settings.numFinalGatheringSamples ? FinalGathering(scene, hit, toViewer, rng)
                                               : indirectPhotonMap->RadianceEstimate(hit, toViewer);
    }

    // Add caustics
    if (!causticPhotonMap->Empty())
    {
        L+= causticPhotonMap->RadianceEstimate(hit, toViewer);
    }

    if (ray.depth+1 < settings.specularDepth)
    {
        AtomicCounter& numSecondaryRays = stats.Counter("Rays", "Secondary rays traced");

        // Trace a ray for specular reflection
        BsdfSamplingRecord brec1(hit, rng, BsdfComponent::DeltaReflection);
        hit.material->SampleBsdf(toViewer, brec1);
        if ((brec1.pdf > 0.0f) && !brec1.value.IsBlack())
        {
            Ray r(hit.point, brec1.sampledDirection);
            r.depth = ray.depth+1;
            L += brec1.value * AbsDotProduct(brec1.sampledDirection, hit.uvn.N()) * Radiance(scene, r, rng, opacity);
            ++numSecondaryRays;
        }

        // Trace a ray for specular transmission
        BsdfSamplingRecord brec2(hit, rng, BsdfComponent::DeltaTransmission);
        hit.material->SampleBsdf(toViewer, brec2);
        if ((brec2.pdf > 0.0f) && !brec2.value.IsBlack())
        {
            Ray r(hit.point, brec2.sampledDirection);
            r.depth = ray.depth+1;
            L += brec2.value * AbsDotProduct(brec2.sampledDirection, hit.uvn.N()) * Radiance(scene, r, rng, opacity);
            ++numSecondaryRays;
        }
    }

    return L;
};
}
