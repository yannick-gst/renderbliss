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

#include <UnitTest++.h>
#include "renderbliss/Math/Sampling/SamplingFunctions.h"
#include "renderbliss/Math/MersenneTwister.h"

namespace
{
TEST(CheckStratifiedSampling1D)
{
    size_t nSamples = 30;
    renderbliss::SampleList1D samples;
    renderbliss::MersenneTwister rng;

    GenerateStratifiedSamples(rng, samples, nSamples);

    CHECK(samples.size() == nSamples);

    size_t sampleCount = samples.size();
    for (size_t i = 0; i < sampleCount; ++i)
    {
        CHECK(samples[i] > static_cast<renderbliss::real>(i)/30);
        CHECK(samples[i] < static_cast<renderbliss::real>(i+1)/30);
    }
}

TEST(CheckStratifiedSampling2D)
{
    size_t width = 30;
    size_t height = 54;

    renderbliss::SampleList2D samples;
    renderbliss::MersenneTwister rng;

    GenerateStratifiedSamples(rng, samples, width, height);

    CHECK(samples.size() == width*height);

    size_t current = 0;
    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            CHECK(samples[current][0] > static_cast<renderbliss::real>(i)/width);
            CHECK(samples[current][0] < static_cast<renderbliss::real>(i+1)/width);

            CHECK(samples[current][1] > static_cast<renderbliss::real>(j)/height);
            CHECK(samples[current][1] < static_cast<renderbliss::real>(j+1)/height);

            ++current;
        }
    }
}
}
