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

#ifndef RENDERBLISS_BSDF_H
#define RENDERBLISS_BSDF_H

#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
// Helper declarations for Bidirectional Scattering Distribution Function modelling and sampling

struct BsdfComponent : NonConstructible
{
    enum Enum
    {
        // A diffuse bsdf scatters incoming light equally in all directions
        DiffuseReflection   = 0x01,
        DiffuseTransmission = 0x02,

        // A bsdf modeled with a delta distribution scatters light in a single outgoing direction
        DeltaReflection     = 0x04,
        DeltaTransmission   = 0x08,

        // A glossy bsdf scatters incoming light preferentially in a set of directions
        GlossyReflection    = 0x10,
        GlossyTransmission  = 0x20,
    };
};

struct BsdfCombinedFlags : NonConstructible
{
    enum Enum
    {
        Diffuse     = BsdfComponent::DiffuseReflection   | BsdfComponent::DiffuseTransmission,
        Delta       = BsdfComponent::DeltaReflection     | BsdfComponent::DeltaTransmission,
        Glossy      = BsdfComponent::GlossyReflection    | BsdfComponent::GlossyTransmission,
        Reflection  = BsdfComponent::DiffuseReflection   | BsdfComponent::GlossyReflection  | BsdfComponent::DeltaReflection,
        Transmission= BsdfComponent::DiffuseTransmission | BsdfComponent::GlossyTransmission | BsdfComponent::DeltaTransmission,
        All         = Reflection | Transmission
    };
};

struct Intersection;
class  MersenneTwister;

struct BsdfSamplingRecord : boost::noncopyable
{
    BsdfSamplingRecord(const Intersection& hit, real samples[3], int flagsToSample);
    BsdfSamplingRecord(const Intersection& hit, MersenneTwister& rng, int flagsToSample);
    Spectrum value;
    Vector3 sampledDirection; // In world space
    real pdf;
    // Canonical randoms used to sample the BSDF
    // The first value is used to sample a component, and the others to sample a direction
    boost::array<real, 3> samples;
    size_t sampledComponentIndex;
    const Intersection& hit;
    const int flagsToSample;
    void Reset();
};
}

#endif
