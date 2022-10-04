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

#ifndef RENDERBLISS_SAMPLINGFUNCTIONS_H
#define RENDERBLISS_SAMPLINGFUNCTIONS_H

#include "renderbliss/Math/Sampling/Sampling.h"

namespace renderbliss
{
class MersenneTwister;

// 1D sampling
void GenerateStratifiedSamples(MersenneTwister& rng, SampleList1D& samples, size_t nSamples);
void Shift(SampleList1D& samples, real x);

// 2D sampling
void GenerateStratifiedSamples(MersenneTwister& rng, SampleList2D& samples, size_t width, size_t height);
void Shift(SampleList2D& samples, real x, real y);

// N-d sampling
void GenerateLatinHypercubeSamples(MersenneTwister& rng, std::vector<real>& samples, size_t nSamples, size_t nDimensions);

// Multiple importance sampling
real BalanceHeuristic(uint32 fNumSamples, real fPdf, uint32 gNumSamples, real gPdf);
real PowerHeuristic(uint32 fNumSamples, real fPdf, uint32 gNumSamples, real gPdf);
}

#endif
