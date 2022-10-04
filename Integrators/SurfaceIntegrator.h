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

#ifndef RENDERBLISS_SURFACE_INTEGRATOR_H
#define RENDERBLISS_SURFACE_INTEGRATOR_H

#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Interfaces/IIntegrator.h"

namespace renderbliss
{
struct Intersection;
class  MersenneTwister;
struct Ray;
class  Scene;
class  StatsTracker;

// A base class for evaluating the surface rendering equation
class SurfaceIntegrator : public IIntegrator
{
public:

    SurfaceIntegrator(StatsTracker& stats) : IIntegrator(stats) {}
    // Returns the radiance along a ray being cast into the scene
    virtual Spectrum Radiance(const Scene& scene, const Ray& ray, MersenneTwister& rng, real& opacity) const = 0;
};
}

#endif
