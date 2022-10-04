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

#ifndef RENDERBLISS_STEP_FUNCTION_SAMPLER_H
#define RENDERBLISS_STEP_FUNCTION_SAMPLER_H

#include <vector>
#include "renderbliss/Types.h"

namespace renderbliss
{
// Helper class to sample the values of a step function with a probability proportional to their relative weight
class StepFunctionSampler
{
public:

    StepFunctionSampler(const std::vector<real>& function);
    real FunctionIntegral() const;
    real FunctionValue(size_t index) const;
    size_t SampleIndex(real canonicalRandom) const;
    size_t StepCount() const;

private:

    std::vector<real> normalizedCdf; // Cumulative distribution function divided by the function integral
    real integral;
};
}

#endif
