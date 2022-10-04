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

#include "renderbliss/Types.h"
#include "renderbliss/Integrators/WhittedIntegrator.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Scene.h"
#include "renderbliss/Integrators/DirectIlluminationUtils.h"
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Lights/Luminaire.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Utils/PropertyMap.h"
#include "renderbliss/Utils/StatsTracker.h"

namespace renderbliss
{
WhittedIntegrator::Settings::Settings(const PropertyMap& props)
{
    props.Get<uint32>("specular_depth", 4, specularDepth);
    props.Get<uint32>("shadow_rays", 4, numShadowRays);
}

WhittedIntegrator::WhittedIntegrator(const PropertyMap& props, StatsTracker& stats) : SurfaceIntegrator(stats), settings(props)
{
    stats.AddCounter("Intersections", "Intersection tests");
    stats.AddCounter("Intersections", "Intersection hits");
    stats.AddCounter("Rays", "Primary rays traced");
    stats.AddCounter("Rays", "Secondary rays traced");
    stats.AddCounter("Rays", "Shadow rays traced");
}

void WhittedIntegrator::PreProcess(const Scene& scene, JobScheduler&)
{
    powerDistribution.reset(PowerDistribution(scene).release());
}

Spectrum WhittedIntegrator::Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const
{
    Spectrum L(Spectrum::black);

    if (ray.depth == 0)
    {
        ++stats.Counter("Rays", "Primary rays traced");
    }

    Intersection hit;
    if (!scene.Intersects(ray, hit))
    {
        return L;
    }

    Vector3 toViewer = -ray.Direction().GetNormalized();

    if (hit.emitter)
    {
        L += hit.emitter->EmittedRadiance(hit.uvn.N(), toViewer);
    }

    L += DirectIllumination(scene, toViewer, hit, powerDistribution.get(), settings.numShadowRays, rng, opacity, stats.Counter("Rays", "Shadow rays traced"));

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
}
}
