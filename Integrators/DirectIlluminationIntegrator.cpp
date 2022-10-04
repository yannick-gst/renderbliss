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

#include "renderbliss/Integrators/DirectIlluminationIntegrator.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Scene.h"
#include "renderbliss/Integrators/DirectIlluminationUtils.h"
#include "renderbliss/Interfaces/ILight.h"
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Sampling/SamplingFunctions.h"
#include "renderbliss/Utils/PropertyMap.h"
#include "renderbliss/Utils/StatsTracker.h"

namespace renderbliss
{
DirectIlluminationIntegrator::Settings::Settings(const PropertyMap& props)
{
    props.Get<uint32>("shadow_rays", 4, numShadowRays);
}

DirectIlluminationIntegrator::DirectIlluminationIntegrator(const PropertyMap& props, StatsTracker& stats) : SurfaceIntegrator(stats), settings(props)
{
    stats.AddCounter("Intersections", "Intersection tests");
    stats.AddCounter("Intersections", "Intersection hits");
    stats.AddCounter("Rays", "Primary rays traced");
    stats.AddCounter("Rays", "Shadow rays traced");
}

void DirectIlluminationIntegrator::PreProcess(const Scene& scene)
{
    powerDistribution.reset(PowerDistribution(scene).release());
}

Spectrum DirectIlluminationIntegrator::Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const
{
    Intersection hit;
    Spectrum L;
    if (scene.Intersects(ray, hit))
    {
        L = DirectIllumination(scene, -ray.Direction().GetNormalized(), hit, powerDistribution.get(), settings.numShadowRays, rng, opacity, stats.Counter("Rays", "Shadow rays traced"));
    }
    ++stats.Counter("Rays", "Primary rays traced");
    return L;
}
}
