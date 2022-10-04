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

#include "renderbliss/Lights/StepFunctionSampler.h"
#include <algorithm>

namespace renderbliss
{
StepFunctionSampler::StepFunctionSampler(const std::vector<real>& function) : integral(0.0f)
{
    RB_ASSERT(!function.empty());

    size_t numSteps = function.size();
    normalizedCdf.reserve(numSteps + 1);
    normalizedCdf.push_back(0.0f);

    for (size_t i = 0; i < numSteps; ++i)
    {
        normalizedCdf.push_back(normalizedCdf[i] + function[i]);
    }

    integral = normalizedCdf[numSteps];

    for (size_t i = 1; i <= numSteps; ++i)
    {
        normalizedCdf[i] /= integral;
    }
}

real StepFunctionSampler::FunctionIntegral() const
{
    return integral;
}

real StepFunctionSampler::FunctionValue(size_t index) const
{
    RB_ASSERT(index+1 < normalizedCdf.size());
    return integral*(normalizedCdf[index+1] - normalizedCdf[index]);
}

size_t StepFunctionSampler::SampleIndex(real canonicalRandom) const
{
    RB_ASSERT(canonicalRandom >= 0.0f);
    RB_ASSERT(canonicalRandom <  1.0f);
    std::vector<real>::const_iterator ubIter = std::upper_bound(normalizedCdf.begin(), normalizedCdf.end(), canonicalRandom);
    RB_ASSERT(ubIter != normalizedCdf.end());
    return ubIter-normalizedCdf.begin()-1;
}

size_t StepFunctionSampler::StepCount() const
{
    return normalizedCdf.size() - 1;
}
}
