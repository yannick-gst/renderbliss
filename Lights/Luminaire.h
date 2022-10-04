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

#ifndef RENDERBLISS_LUMINAIRE_H
#define RENDERBLISS_LUMINAIRE_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "renderbliss/Interfaces/ILight.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Lights/StepFunctionSampler.h"

namespace renderbliss
{
class TrianglePrimitive;
typedef boost::shared_ptr<const TrianglePrimitive> TrianglePrimitiveConstPtr;
typedef std::vector<TrianglePrimitiveConstPtr> TrianglePrimitiveList;

// A light-emitting object defined by attaching a shape to an emission profile
class Luminaire : public ILight
{
public:

    Luminaire(const TrianglePrimitiveList& triangles, const Spectrum& radiantExitance, real controlExponent=1.0f); // The control exponent should be >= 1

    // Returns the PDF with respect to solid angle for sampling a particular direction towards
    // a surface point receiving light. The sampling direction points away towards the light sourcce.
    virtual real Pdf(const Vector3& surfacePoint, const Vector3& toLight) const;

    // Returns the power of the light source in Watt.
    virtual Spectrum Power() const;

    // Samples the light source in order to obtain an incident direction.
    virtual void SampleIncidentDirection(const Intersection& hit, LightSamplingRecord& lrec) const;

    // Samples the light source in order to generate a ray leaving the light.
    virtual void SampleOutgoingRay(const real canonicalRandom[5], Vector3& origin, Vector3& direction, Vector3& lightNormal, real& pdf) const;

    // Returns the radiance emitted toward in a viewing direction.
    virtual Spectrum EmittedRadiance(const Vector3& lightNormal, const Vector3& toViewer) const;

private:

    Spectrum radiantExitance;
    TrianglePrimitiveList triangles;
    boost::scoped_ptr<StepFunctionSampler> areaDistribution;
    // With a control exponent of 1, the luminaire has a diffuse emission profile.
    // With a large control exponent, the light is concentrated near the surface normal of the emission point.
    real controlExponent;
};
}

#endif
