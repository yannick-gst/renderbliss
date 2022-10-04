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
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Colour/RGB.h"
#include "renderbliss/Math/MathUtils.h"
#include <boost/random.hpp>

#ifndef SPECTRUM_IS_RGB
namespace
{
using namespace renderbliss; 

boost::mt19937 rng;
boost::uniform_real<float> dist(0,1);
boost::variate_generator<boost::mt19937, boost::uniform_real<float> > die(rng, dist);

struct SpectrumFixture
{
    Spectrum colour;
    SpectrumFixture()
    {
        for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
        {
            colour[i] = die();
        }
    }
};

TEST(CheckBlack)
{
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(0, Spectrum::black[i], Epsilon());
    }
}

TEST(CheckWhite)
{
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(1, Spectrum::white[i], Epsilon());
    }
}

TEST(CheckConstructorFromSingleRealParameter)
{
    float val = die();
    Spectrum colour(val);
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(val, colour[i], Epsilon());
    }
}

TEST_FIXTURE(SpectrumFixture, CheckClamp)
{
    colour.Clamp();
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK(colour[i] >= 0.0f);
        CHECK(colour[i] <= 1.0f);
    }
}

TEST(CheckIsBlack)
{
    Spectrum colour(1);
    CHECK(!colour.IsBlack());

    Spectrum black(0);
    CHECK(black.IsBlack());
}

TEST(CheckIsWhite)
{
    Spectrum colour(0.5f);
    CHECK(!colour.IsWhite());

    Spectrum white(1.0f);
    CHECK(white.IsWhite());
}

TEST_FIXTURE(SpectrumFixture, CheckOperatorNegation)
{
    Spectrum neg = -colour;
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(neg[i], -colour[i], Epsilon());
    }
}

TEST_FIXTURE(SpectrumFixture, CheckOperatorPlusEqual)
{
    Spectrum b(die());
    Spectrum orig = colour;
    colour += b;
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(colour[i], orig[i]+b[i], Epsilon());
    }
}

TEST_FIXTURE(SpectrumFixture, CheckOperatorMinusEqual)
{
    Spectrum b(die());
    Spectrum orig = colour;
    colour -= b;
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(colour[i], orig[i]-b[i], Epsilon());
    }
}

TEST_FIXTURE(SpectrumFixture, CheckOperatorTimesEqual)
{
    Spectrum b(die());
    Spectrum orig = colour;
    colour *= b;
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(colour[i], orig[i]*b[i], Epsilon());
    }
}

TEST_FIXTURE(SpectrumFixture, CheckOperatorDivEqual)
{
    Spectrum b(die());
    Spectrum orig = colour;
    colour /= b;
    for (size_t i = 0; i < Spectrum::SampleCount(); ++i)
    {
        CHECK_CLOSE(colour[i], orig[i]/b[i], Epsilon());
    }
}
}
#endif
