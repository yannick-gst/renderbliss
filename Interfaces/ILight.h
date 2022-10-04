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

#ifndef RENDERBLISS_ILIGHT_H
#define RENDERBLISS_ILIGHT_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Math/Geometry/Ray.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Math/Sampling/Sampling.h"

namespace renderbliss
{
struct Intersection;
class  MersenneTwister;
class  Scene;
struct Vector2;

// Helper class meant to test light occlusion along a segment or direction
class OcclusionTester
{
public:

    OcclusionTester();
    void InitForOcclusionAlongSegment(const Vector3& start, const Vector3& end);
    void InitForOcclusionAlongDirection(const Vector3& start, const Vector3& end);
    bool FindOcclusion(const Scene& s) const;

private:

    Ray ray;
};

struct LightSamplingRecord : boost::noncopyable
{
    Spectrum emittedRadiance;
    OcclusionTester occlusionTester;
    Vector3 toLight;       // In world space and points towards the light
    Vector3 shadingNormal; // In world space and points away from the light
    real pdf;
    MersenneTwister& rng;
    const Intersection& hit;
    const Sample2D& canonicalRandom;
    LightSamplingRecord(MersenneTwister& rng, const Intersection& hit, const Sample2D& canonicalRandom);
};

// Abstract base class for light sources
class ILight
{
public:

    ILight();
    virtual ~ILight();

    // Returns the PDF with respect to solid angle for sampling a particular direction towards
    // a surface point receiving light. The sampling direction points away towards the light sourcce.
    virtual real Pdf(const Vector3& surfacePoint, const Vector3& toLight) const = 0;

    // Returns the power of the light source in Watt.
    virtual Spectrum Power() const = 0;

    // Samples the light source in order to obtain an incident direction.
    virtual void SampleIncidentDirection(const Intersection& hit, LightSamplingRecord& lrec) const = 0;

    // Samples the light source in order to generate a ray leaving the light.
    virtual void SampleOutgoingRay(const real canonicalRandom[5], Vector3& origin, Vector3& direction, Vector3& lightNormal, real& pdf) const = 0;

    // Returns the radiance emitted toward in a viewing direction.
    virtual Spectrum EmittedRadiance(const Vector3& lightNormal, const Vector3& toViewer) const = 0;
};
}

#endif
