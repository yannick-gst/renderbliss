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

#ifndef RENDERBLISS_IMATERIAL_H
#define RENDERBLISS_IMATERIAL_H

#include "renderbliss/Types.h"
#include "renderbliss/Interfaces/Bsdf.h"

namespace renderbliss
{
class IMaterial
{
public:

    IMaterial();
    virtual ~IMaterial();

    // Naming convention for directions:
    // 'wi' denotes an incident direction in world coordinates
    // 'wo' denotes an outgoing direction in world coordinates
    // 'li' denotes an incident direction in local coordinates
    // 'lo' denotes an outgoing direction in local coordinates

    // Evaluates the BSDF for the input flags.
    // Both directions are of unit length and are expressed in world space.
    Spectrum BsdfCombinedFlagsValue(const Vector3& wi, const Vector3& wo, const Intersection& hit, int flags) const;

    // Returns the total number of BSDF components for this material.
    uint ComponentCount() const;

    // Returns true if one component matches the input BSDF combined flags, and false otherwise.
    bool MatchesFlags(int flags) const;

    // Returns true if the given component matches the input BSDF combined flags, and false otherwise.
    bool MatchesFlags(size_t componentIndex, int flags) const;

    // Returns the number of components matching the input BSDF combined flags.
    uint MatchingComponentCount(int flags) const;

    // Returns the probability of scattering from the incident to the outgoing direction.
    real Pdf(const Vector3& li, const Vector3& lo) const;

    // Samples the BSDF and fills the BSDF sampling record. The incident direction is
    // of unit length, is expressed in world space, and points away from the surface.
    void SampleBsdf(const Vector3& wi, BsdfSamplingRecord& brec) const;

protected:

    enum { MaxComponentCount = 8 };
    boost::array<BsdfComponent::Enum, MaxComponentCount> components;
    uint numComponents;

    // Evaluates the BSDF for the input component.
    // Both directions are of unit-length and are expressed in local space. The incident direction always points away from the surface.
    // The outgoing direction points towards the surface if the sampled component is transmissive. It points away from the surface otherwise.
    virtual Spectrum BsdfComponentValue(const Vector3& li, const Vector3& lo, const Intersection& hit, size_t componentIndex) const = 0;

    // Returns the probability of scattering from the incident to the outgoing direction, given a specific BSDF component.
    // Both directions are of unit-length and are expressed in local space. The incident direction always points away from the surface.
    // The outgoing direction points towards the surface if the sampled component is transmissive. It points away from the surface otherwise.
    virtual real Pdf(const Vector3& li, const Vector3& lo, size_t componentIndex) const = 0;

    // Samples a BSDF component in order to obtain an outgoing direction.
    // The incident direction is of unit-length, expressed in local space, and points away from the surface.
    virtual Vector3 SampleDirection(const Vector3& li, real directionSamples[2], size_t componentIndex) const = 0;
};
}

#endif
