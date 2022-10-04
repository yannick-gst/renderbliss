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
#include "renderbliss/Colour/RGB.h"
#include "renderbliss/Colour/XYZ.h"
#include "renderbliss/Math/MathUtils.h"
#include <boost/random.hpp>

namespace
{
boost::mt19937 rng;
boost::uniform_real<float> dist(-225, 225);
boost::variate_generator<boost::mt19937, boost::uniform_real<float> > die(rng, dist);

struct RGBFixture
{
    float r, g, b;
    renderbliss::RGB colour;
    RGBFixture() : r(die()), g(die()), b(die()), colour(r,g,b) {}
};

TEST(CheckBlack)
{
    CHECK_CLOSE(0, renderbliss::RGB::black.r, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::RGB::black.g, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::RGB::black.b, renderbliss::Epsilon());
}

TEST(CheckWhite)
{
    CHECK_CLOSE(1, renderbliss::RGB::white.r, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::RGB::white.g, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::RGB::white.b, renderbliss::Epsilon());
}

TEST(CheckConstructorFromSingleRealParameter)
{
    float val = die();
    renderbliss::RGB colour(val);

    CHECK_CLOSE(val, colour.r, renderbliss::Epsilon());
    CHECK_CLOSE(val, colour.g, renderbliss::Epsilon());
    CHECK_CLOSE(val, colour.b, renderbliss::Epsilon());
}

TEST(CheckConstructorFromArrayParameter)
{
    float arr[3] =  { die(), die(), die() };
    renderbliss::RGB colour(arr);

    CHECK_CLOSE(arr[0], colour.r, renderbliss::Epsilon());
    CHECK_CLOSE(arr[1], colour.g, renderbliss::Epsilon());
    CHECK_CLOSE(arr[2], colour.b, renderbliss::Epsilon());
}

TEST(CheckConstructorFromThreeRealParameters)
{
    float r=die(), g=die(), b=die();
    renderbliss::RGB colour(r, g, b);

    CHECK_CLOSE(r, colour.r, renderbliss::Epsilon());
    CHECK_CLOSE(g, colour.g, renderbliss::Epsilon());
    CHECK_CLOSE(b, colour.b, renderbliss::Epsilon());
}

TEST(CheckClamp)
{
    renderbliss::RGB colour(die(), die(), die());
    colour.Clamp();

    CHECK(colour.r >= 0.0f);
    CHECK(colour.g >= 0.0f);
    CHECK(colour.b >= 0.0f);

    CHECK(colour.r <= 1.0f);
    CHECK(colour.g <= 1.0f);
    CHECK(colour.b <= 1.0f);
}

TEST(CheckIsBlack)
{
    renderbliss::RGB colour(1,2,3);
    CHECK(!colour.IsBlack());

    renderbliss::RGB black(0,0,0);
    CHECK(black.IsBlack());
}

TEST(CheckIsWhite)
{
    renderbliss::RGB colour(1,2,3);
    CHECK(!colour.IsWhite());

    renderbliss::RGB white(1,1,1);
    CHECK(white.IsWhite());
}

TEST(CheckMaxChannel)
{
    renderbliss::RGB colour(1,2,3);
    CHECK_EQUAL(colour.b, colour.MaxChannel());
}

TEST(CheckMinChannel)
{
    renderbliss::RGB colour(1,2,3);
    CHECK_EQUAL(colour.r, colour.MinChannel());
}

TEST_FIXTURE(RGBFixture, CheckToXYZ)
{
    renderbliss::XYZ colourXYZ = colour.ToXYZ();
    renderbliss::RGB colourRGB = colourXYZ.ToRGB();
    CHECK_CLOSE(colour.r, colourRGB.r, 0.001f);
    CHECK_CLOSE(colour.g, colourRGB.g, 0.001f);
    CHECK_CLOSE(colour.b, colourRGB.b, 0.001f);
}

TEST_FIXTURE(RGBFixture, CheckOperatorIndex)
{
    CHECK_CLOSE(colour[0], colour.r, renderbliss::Epsilon());
    CHECK_CLOSE(colour[1], colour.g, renderbliss::Epsilon());
    CHECK_CLOSE(colour[2], colour.b, renderbliss::Epsilon());
}

TEST_FIXTURE(RGBFixture, CheckOperatorNegation)
{
    CHECK_CLOSE((-colour).r, -r, renderbliss::Epsilon());
    CHECK_CLOSE((-colour).g, -g, renderbliss::Epsilon());
    CHECK_CLOSE((-colour).b, -b, renderbliss::Epsilon());
}

TEST_FIXTURE(RGBFixture, CheckOperatorPlusEqual)
{
    colour += renderbliss::RGB(1,2,3);
    CHECK_CLOSE(colour.r, r+1, renderbliss::Epsilon());
    CHECK_CLOSE(colour.g, g+2, renderbliss::Epsilon());
    CHECK_CLOSE(colour.b, b+3, renderbliss::Epsilon());
}

TEST_FIXTURE(RGBFixture, CheckOperatorMinusEqual)
{
    colour -= renderbliss::RGB(1,2,3);
    CHECK_CLOSE(colour.r, r-1, renderbliss::Epsilon());
    CHECK_CLOSE(colour.g, g-2, renderbliss::Epsilon());
    CHECK_CLOSE(colour.b, b-3, renderbliss::Epsilon());
}

TEST_FIXTURE(RGBFixture, CheckOperatorTimesEqual)
{
    colour *= 4.0f;
    CHECK_CLOSE(colour.r, r*4, renderbliss::Epsilon());
    CHECK_CLOSE(colour.g, g*4, renderbliss::Epsilon());
    CHECK_CLOSE(colour.b, b*4, renderbliss::Epsilon());
}

TEST_FIXTURE(RGBFixture, CheckOperatorDivEqual)
{
    colour /= 4.0f;
    CHECK_CLOSE(colour.r, r/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(colour.g, g/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(colour.b, b/4.0f, renderbliss::Epsilon());
}
}
