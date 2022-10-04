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

#ifndef RENDERBLISS_PHOTON_INTEGRATOR_H
#define RENDERBLISS_PHOTON_INTEGRATOR_H

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Integrators/SurfaceIntegrator.h"
#include "renderbliss/Lights/StepFunctionSampler.h"
#include "renderbliss/Utils/PropertyMap.h"

namespace renderbliss
{
struct Intersection;
class  PhotonMap;
class  IrradiancePhotonMap;
class  StatsTracker;
struct Vector3;

typedef PhotonMap CausticPhotonMap;

// A surface integrator that uses photon mapping to approximate indirect illumination
class PhotonIntegrator : public SurfaceIntegrator
{
public:

    PhotonIntegrator(const PropertyMap& props, StatsTracker& stats);

    // Should be called before rendering a scene
    virtual void PreProcess(const Scene& scene, JobScheduler& scheduler);

    // Returns the radiance along a ray being cast into the scene
    virtual Spectrum Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const;

private:

    struct Settings
    {
        PropertyMap cpmProps; // Caustic photon map properties
        PropertyMap gpmProps; // Global photon map properties
        uint32 specularDepth;
        uint32 numShadowRays;
        uint32 maxPhotonDepth;
        uint32 numFinalGatheringSamples;
        Settings(const PropertyMap& props);
    } settings;
    boost::shared_ptr<CausticPhotonMap> causticPhotonMap;
    boost::shared_ptr<IrradiancePhotonMap> indirectPhotonMap;
    boost::scoped_ptr<StepFunctionSampler> powerDistribution; // Lighting power distribution

    // Implements one-bounce final gathering
    Spectrum FinalGathering(const Scene& scene, const Intersection& hit, const Vector3& outgoing, MersenneTwister& rng) const;
};
}

#endif
