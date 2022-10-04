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

#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Basis3.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Primitives/TrianglePrimitive.h"

namespace renderbliss
{
BsdfSamplingRecord::BsdfSamplingRecord(const Intersection& hit, real samples[3], int flagsToSample)
    : value(Spectrum::black), pdf(0.0f), hit(hit), flagsToSample(flagsToSample)
{
    this->samples[0] = samples[0];
    this->samples[1] = samples[1];
    this->samples[2] = samples[2];
}

BsdfSamplingRecord::BsdfSamplingRecord(const Intersection& hit, MersenneTwister& rng, int flagsToSample)
    : value(Spectrum::black), pdf(0.0f), hit(hit), flagsToSample(flagsToSample)
{
    samples[0] = rng.CanonicalRandom();
    samples[1] = rng.CanonicalRandom();
    samples[2] = rng.CanonicalRandom();
}

void BsdfSamplingRecord::Reset()
{
    value = Spectrum::black;
    pdf = 0.0f;
}

IMaterial::IMaterial()
{
}

IMaterial::~IMaterial()
{
}

Spectrum IMaterial::BsdfCombinedFlagsValue(const Vector3& wi, const Vector3& wo, const Intersection& hit, int flags) const
{
    RB_ASSERT(numComponents);

    // The orientation of the incident and outgoing directions with respect to the
    // the normal determines whether reflective or transmissive bdfs will be ignored
    Vector3 geometricNormal = hit.triangle->GeometricNormal();
    real dotI = DotProduct(wi, geometricNormal);
    real dotO = DotProduct(wo, geometricNormal);
    flags &= (dotI*dotO) > 0.0f ? ~BsdfCombinedFlags::Transmission : ~BsdfCombinedFlags::Reflection;

    // Accumulate the BDF
    Spectrum result(Spectrum::black);
    Vector3 lo = hit.uvn.ToLocal(wo);
    Vector3 li = hit.uvn.ToLocal(wi);
    for (size_t i = 0; i < numComponents; ++i)
    {
        if (components[i] & flags)
        {
            result += BsdfComponentValue(li, lo, hit, i);
        }
    }

    return result;
}

uint IMaterial::ComponentCount() const
{
    return numComponents;
}

bool IMaterial::MatchesFlags(int flags) const
{
    RB_ASSERT(numComponents <= MaxComponentCount);
    for (size_t i = 0; i < numComponents; ++i)
    {
        if (components[i] & flags) return true;
    }
    return false;
}

bool IMaterial::MatchesFlags(size_t componentIndex, int flags) const
{
    RB_ASSERT(componentIndex < numComponents);
    RB_ASSERT(numComponents <= MaxComponentCount);
    return (components[componentIndex] & flags) ? true : false;
}

uint IMaterial::MatchingComponentCount(int flags) const
{
    RB_ASSERT(numComponents <= MaxComponentCount);
    uint result = 0;
    for (size_t i = 0; i < numComponents; ++i)
    {
        if (components[i] & flags) ++result;
    }
    return result;
}

real IMaterial::Pdf(const Vector3& li, const Vector3& lo) const
{
    real pdf = 0.0f;
    for (size_t i = 0; i < numComponents; ++i)
    {
        pdf += Pdf(li, lo, i);
    }
    return pdf;
}

void IMaterial::SampleBsdf(const Vector3& wi, BsdfSamplingRecord& brec) const
{
    RB_ASSERT(numComponents);

    brec.Reset();

    // First we need to select a component to sample. We do it by randomly
    // picking an occurence of component matching the flags we will be sampling
    uint numMatching = MatchingComponentCount(brec.flagsToSample);
    if (!numMatching) return;
    size_t componentIndex = 0;
    uint pick = std::min(numMatching-1, static_cast<uint>(brec.samples[0] * numMatching));
    for (; componentIndex < numComponents; ++componentIndex)
    {
        if ((components[componentIndex] & brec.flagsToSample) && (pick-- == 0))
        {
            brec.sampledComponentIndex = componentIndex;
            break;
        }
    }

    // Next, we sample an outgoing direction
    Vector3 li = brec.hit.uvn.ToLocal(wi);
    Vector3 lo = SampleDirection(li, &(brec.samples[1]), brec.sampledComponentIndex);

    // Finally we compute the PDF and BSDF

    if (components[brec.sampledComponentIndex] & BsdfCombinedFlags::Delta)
    {
        brec.value = BsdfComponentValue(li, lo, brec.hit, brec.sampledComponentIndex);
        brec.pdf = 1.0f; // Since it is a delta distribution
    }
    else
    {
        brec.pdf = Pdf(li, lo, brec.sampledComponentIndex);
        if (brec.pdf == 0.0f)
        {
            brec.value = Spectrum::black;
            return;
        }
        else
        {
            brec.value = BsdfComponentValue(li, lo, brec.hit, brec.sampledComponentIndex);
        }

        // For non-delta distributions, the BSDF and the PDF will be accumulated over the
        // components matching the sampled component in the record, and the PDF will be averaged.
        for (size_t i = 0; i < numComponents; ++i)
        {
            if (components[i] & brec.flagsToSample)
            {
                if (i != brec.sampledComponentIndex)
                {
                    brec.pdf += Pdf(li, lo, i);
                    brec.value += BsdfComponentValue(li, lo, brec.hit, i);
                }
            }
        }

        if (numMatching > 1)
        {
            brec.pdf /= numMatching;
        }
    }

    brec.sampledDirection = brec.hit.uvn.ToWorld(lo);
}
}
