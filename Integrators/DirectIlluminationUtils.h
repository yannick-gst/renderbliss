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

#ifndef RENDERBLISS_DIRECT_ILLUMINATION_UTILS_H
#define RENDERBLISS_DIRECT_ILLUMINATION_UTILS_H

#include <memory>
#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"

namespace renderbliss
{
class  AtomicCounter;
struct Intersection;
class  MersenneTwister;
class  Scene;
class  StepFunctionSampler;
struct Vector3;

std::auto_ptr<StepFunctionSampler> PowerDistribution(const Scene& scene);

Spectrum DirectIllumination(const Scene& scene, const Vector3& toViewer, const Intersection& hit,
                            const StepFunctionSampler* powerDistribution, uint32 numShadowRays,
                            MersenneTwister& rng, real& opacity, AtomicCounter& numTracedShadowRays);

Spectrum LightingPower(const Scene& scene);
}

#endif
