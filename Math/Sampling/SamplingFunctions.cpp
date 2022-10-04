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

#include "renderbliss/Math/Sampling/SamplingFunctions.h"
#include <algorithm>
#include <functional>
#include <boost/function.hpp>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MersenneTwister.h"

namespace renderbliss
{
// 1D sampling

void GenerateStratifiedSamples(MersenneTwister& rng, SampleList1D& samples, size_t nSamples)
{
    samples.clear();
    if (nSamples)
    {
        samples.reserve(nSamples);
        real inv = 1.0f/nSamples;

        for (uint i = 0; i < nSamples; ++i)
        {
            samples.push_back(inv * (i + rng.CanonicalRandom()));
        }
    }
}

void Shift(SampleList1D& samples, real x)
{
    foreach (Sample1D& sample, samples)
    {
        sample += x;
    }
}

// 2D sampling

void GenerateStratifiedSamples(MersenneTwister& rng, SampleList2D& samples, size_t width, size_t height)
{
    samples.clear();
    if (width*height)
    {
        samples.reserve(width*height);

        Sample2D sample;

        for (uint i = 0; i < width; ++i)
        {
            for (uint j = 0; j < height; ++j)
            {
                sample[0] = (i + rng.CanonicalRandom()) / width;
                sample[1] = (j + rng.CanonicalRandom()) / height;
                samples.push_back(sample);
            }
        }
    }
}

void Shift(SampleList2D& samples, real x, real y)
{
    foreach (Sample2D& sample, samples)
    {
        sample[0] += x;
        sample[1] += y;
    }
}

// N-d sampling
void GenerateLatinHypercubeSamples(MersenneTwister& rng, std::vector<real>& samples, size_t nSamples, size_t nDimensions)
{
    std::vector<real>(nSamples*nDimensions, 0.0f).swap(samples);
    real delta = 1.0f/nSamples;
    // Generate samples along diagonal
    for (size_t i = 0; i < nSamples; ++i)
    {
        for (size_t j = 0; j < nDimensions; ++j)
        {
            real s = (i + rng.CanonicalRandom())*delta;
            samples[nDimensions*i + j] = s;
        }
    }
    // Permute samples in each dimension
    for (size_t j = 0; j < nDimensions; ++j)
    {
        for (size_t i = 0; i < nSamples; ++i)
        {
            size_t other = i + rng.RandomUint(nSamples-i-1);
            std::swap(samples[nDimensions*i + j], samples[nDimensions*other + j]);
        }
    }
}

// Multiple importance sampling
real BalanceHeuristic(uint32 fNumSamples, real fPdf, uint32 gNumSamples, real gPdf)
{
    RB_ASSERT(fNumSamples && gNumSamples);
    RB_ASSERT((fPdf > 0.0f) && (gPdf > 0.0f));
    return (fNumSamples*fPdf) / ((fNumSamples*fPdf) + (gNumSamples*gPdf));
}

real PowerHeuristic(uint32 fNumSamples, real fPdf, uint32 gNumSamples, real gPdf)
{
    RB_ASSERT(fNumSamples && gNumSamples);
    RB_ASSERT((fPdf > 0.0f) && (gPdf > 0.0f));
    real f = fNumSamples*fPdf;
    real g = gNumSamples*gPdf;
    return (f*f) / (f*f + g*g);
}
}
