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

#include "renderbliss/Integrators/DirectIlluminationUtils.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Scene.h"
#include "renderbliss/Interfaces/ILight.h"
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Lights/StepFunctionSampler.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Math/Sampling/SamplingFunctions.h"
#include "renderbliss/Utils/AtomicOps.h"

namespace renderbliss
{
std::auto_ptr<StepFunctionSampler> PowerDistribution(const Scene& scene)
{
    std::vector<real> powerFunction;
    foreach (const LightConstPtr& l, scene.Lights())
    {
        powerFunction.push_back(l->Power().Luminance());
    }
    return std::auto_ptr<StepFunctionSampler>(new StepFunctionSampler(powerFunction));
}

Spectrum DirectIllumination(const Scene& scene, const Vector3& toViewer, const Intersection& hit,
                            const StepFunctionSampler* powerDistribution, uint32 numShadowRays,
                            MersenneTwister& rng, real& opacity, AtomicCounter& numTracedShadowRays)
{
    Spectrum Ld(Spectrum::black);
    const LightPtrList& lights = scene.Lights();
    if (!hit.triangle || !hit.material || lights.empty() || !numShadowRays)
    {
        return Ld;
    }

    // Request light samples
    std::vector<real> lightSamples;
    GenerateLatinHypercubeSamples(rng, lightSamples, numShadowRays, 2);

    // Select light source to sample
    size_t pick = powerDistribution ? powerDistribution->SampleIndex(rng.CanonicalRandom()) : rng.RandomUint(lights.size()-1);
    RB_ASSERT((pick >= 0) && (pick < lights.size()));
    LightConstPtr lightToSample = lights[pick];

    // Accumulate the direct illumination
    Sample2D lightSample;
    size_t sampleIndex = 0;
    for (size_t i = 0; i < numShadowRays; ++i, sampleIndex+=2)
    {
        lightSample[0] = lightSamples[sampleIndex];
        lightSample[1] = lightSamples[sampleIndex+1];

        // We pay the computational expense of shooting a shadow ray only if
        // the light pdf, the BSDF and the emitted radiance are not zero

        LightSamplingRecord lrec(rng, hit, lightSample);

        lightToSample->SampleIncidentDirection(hit, lrec);

        const Spectrum& Le = lrec.emittedRadiance;

        if ((lrec.pdf == 0.0f) || Le.IsBlack()) continue;

        Spectrum bsdfValue = hit.material->BsdfCombinedFlagsValue(lrec.toLight, toViewer, hit, BsdfCombinedFlags::All);

        if (bsdfValue.IsBlack()) continue;

        if (!lrec.occlusionTester.FindOcclusion(scene))
        {
            Ld += (Le*bsdfValue*AbsDotProduct(hit.uvn.N(), lrec.toLight)) / lrec.pdf;
        }

        ++numTracedShadowRays;
    }

    opacity *= 1.0f;
    Ld *= static_cast<real>(lights.size())/numShadowRays;
    return Ld;
}

Spectrum LightingPower(const Scene& scene)
{
    Spectrum power;
    foreach (const LightConstPtr& l, scene.Lights())
    {
        if (l.get()) power += l->Power();
    }
    return power;
}
}
