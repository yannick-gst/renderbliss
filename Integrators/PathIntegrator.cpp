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

#include "renderbliss/Integrators/PathIntegrator.h"
#include <algorithm>
#include <cstdlib>
#include "renderbliss/Scene.h"
#include "renderbliss/Integrators/DirectIlluminationUtils.h"
#include "renderbliss/Interfaces/ILight.h"
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Lights/Luminaire.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Sampling/SamplingFunctions.h"
#include "renderbliss/Utils/PropertyMap.h"
#include "renderbliss/Utils/StatsTracker.h"

namespace renderbliss
{
PathIntegrator::Settings::Settings(const PropertyMap& props)
{
    props.Get<uint32>("path_depth", 4, maxPathDepth);
    props.Get<uint32>("shadow_rays", 4, numShadowRays);
}

PathIntegrator::PathIntegrator(const PropertyMap& props, StatsTracker& stats) : SurfaceIntegrator(stats), settings(props)
{
    stats.AddCounter("Intersections", "Intersection tests");
    stats.AddCounter("Intersections", "Intersection hits");
    stats.AddCounter("Rays", "Primary rays traced");
    stats.AddCounter("Rays", "Secondary rays traced");
    stats.AddCounter("Rays", "Shadow rays traced");
}

Spectrum PathIntegrator::IndirectIllumination(const Scene& scene, const Vector3& toViewer, const Intersection& hit, uint32 depth, MersenneTwister& rng, real& opacity) const
{
    if (depth+1 >= settings.maxPathDepth) { return Spectrum::black; }

    BsdfSamplingRecord brec(hit, rng, BsdfCombinedFlags::All);
    hit.material->SampleBsdf(toViewer, brec);

    if ((brec.pdf <= 0.0f) || (brec.value.IsBlack())) { return Spectrum::black; }

    Ray ray(hit.point, brec.sampledDirection);
    ray.depth = depth+1;
    real dot = AbsDotProduct(hit.uvn.N(), brec.sampledDirection);

    return dot*brec.value*Radiance(scene, ray, rng, opacity) / brec.pdf;
}

void PathIntegrator::PreProcess(const Scene& scene, JobScheduler&)
{
    powerDistribution.reset(PowerDistribution(scene).release());
}

Spectrum PathIntegrator::Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const
{
    Spectrum L(Spectrum::black);

    (ray.depth == 0) ? ++stats.Counter("Rays", "Primary rays traced") : ++stats.Counter("Rays", "Secondary rays traced");

    Intersection hit;
    if (!scene.Intersects(ray, hit))
    {
        return L;
    }

    Vector3 toViewer = -ray.Direction().GetNormalized();

    if ((ray.depth==0) && hit.emitter)
    {
        L += hit.emitter->EmittedRadiance(hit.uvn.N(), toViewer);
    }

    L += DirectIllumination(scene, toViewer, hit, powerDistribution.get(), settings.numShadowRays, rng, opacity, stats.Counter("Rays", "Shadow rays traced"));
    L += IndirectIllumination(scene, toViewer, hit, ray.depth, rng, opacity);

    return L;
}
}
