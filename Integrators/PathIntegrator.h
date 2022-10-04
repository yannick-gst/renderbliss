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

#ifndef RENDERBLISS_PATH_INTEGRATOR_H
#define RENDERBLISS_PATH_INTEGRATOR_H

#include <boost/scoped_ptr.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Integrators/SurfaceIntegrator.h"
#include "renderbliss/Lights/StepFunctionSampler.h"

namespace renderbliss
{
class PropertyMap;
class StatsTracker;
struct Vector3;

// A surface integrator implementing stochastic path tracing
// Mostly intended for rendering reference images
class PathIntegrator : public SurfaceIntegrator
{
public:

    PathIntegrator(const PropertyMap& props, StatsTracker& stats);

    // Should be called before rendering a scene
    virtual void PreProcess(const Scene& scene, JobScheduler&);

    // Returns the radiance along a ray being cast into the scene
    virtual Spectrum Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const;

private:

    struct Settings
    {
        uint32 maxPathDepth;
        uint32 numShadowRays;
        Settings(const PropertyMap& props);
    } settings;
    boost::scoped_ptr<StepFunctionSampler> powerDistribution; // Lighting power distribution

    // Returns the indirect illumination along a ray being cast into the scene
    Spectrum IndirectIllumination(const Scene& scene, const Vector3& toViewer, const Intersection& hit, uint32 depth, MersenneTwister& rng, real& opacity) const;
};
}

#endif
