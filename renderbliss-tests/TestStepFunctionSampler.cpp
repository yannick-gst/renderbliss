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
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Lights/StepFunctionSampler.h"

namespace
{
using namespace renderbliss;

struct SfsFixture
{
    std::vector<real> function;
    boost::scoped_ptr<StepFunctionSampler> sfs;
    SfsFixture()
    {
        for (int i = 0; i < 10; ++i)
        {
            function.push_back(static_cast<real>(i+1));
        }
        sfs.reset(new StepFunctionSampler(function));
    }
};

TEST_FIXTURE(SfsFixture, CheckFunctionIntegral)
{
    CHECK_CLOSE(55.0f, sfs->FunctionIntegral(), Epsilon());
}

TEST_FIXTURE(SfsFixture, CheckFunctionValue)
{
    for (size_t i = 0; i < 10; ++i)
    {
        CHECK_CLOSE(function[i], sfs->FunctionValue(i), Epsilon());
    }
}

TEST_FIXTURE(SfsFixture, CheckSampleIndex)
{
    CHECK_EQUAL(static_cast<size_t>(0), sfs->SampleIndex(0.0f));
    CHECK_EQUAL(static_cast<size_t>(1), sfs->SampleIndex(0.03f));
    CHECK_EQUAL(static_cast<size_t>(2), sfs->SampleIndex(0.1f));
    CHECK_EQUAL(static_cast<size_t>(3), sfs->SampleIndex(0.18f));
    CHECK_EQUAL(static_cast<size_t>(4), sfs->SampleIndex(0.25f));
    CHECK_EQUAL(static_cast<size_t>(5), sfs->SampleIndex(0.38f));
    CHECK_EQUAL(static_cast<size_t>(6), sfs->SampleIndex(0.49f));
    CHECK_EQUAL(static_cast<size_t>(7), sfs->SampleIndex(0.65f));
    CHECK_EQUAL(static_cast<size_t>(8), sfs->SampleIndex(0.80f));
    CHECK_EQUAL(static_cast<size_t>(9), sfs->SampleIndex(0.901f));
}
}
