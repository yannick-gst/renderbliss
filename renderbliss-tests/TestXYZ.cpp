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
#define _USE_MATH_DEFINES
#include <cmath>
#include <boost/random.hpp>
#include "renderbliss/Colour/RGB.h"
#include "renderbliss/Colour/XYZ.h"
#include "renderbliss/Math/MathUtils.h"

namespace
{
boost::mt19937 rng;
boost::uniform_real<float> dist(-225, 225);
boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > die(rng, dist);

struct XYZFixture
{
    float x, y, z;
    renderbliss::XYZ colour;
    XYZFixture() : x(die()), y(die()), z(die()), colour(x,y,z) {}
};

TEST(CheckConstructorFromSingleRealParameter)
{
    float val = die();
    renderbliss::XYZ colour(val);

    CHECK_CLOSE(val, colour.x, renderbliss::Epsilon());
    CHECK_CLOSE(val, colour.y, renderbliss::Epsilon());
    CHECK_CLOSE(val, colour.z, renderbliss::Epsilon());
}

TEST(CheckConstructorFromArrayParameter)
{
    float arr[3] =  { die(), die(), die() };
    renderbliss::XYZ colour(arr);

    CHECK_CLOSE(arr[0], colour.x, renderbliss::Epsilon());
    CHECK_CLOSE(arr[1], colour.y, renderbliss::Epsilon());
    CHECK_CLOSE(arr[2], colour.z, renderbliss::Epsilon());
}

TEST(CheckConstructorFromThreeRealParameters)
{
    float x=die(), y=die(), z=die();
    renderbliss::XYZ colour(x, y, z);

    CHECK_CLOSE(x, colour.x, renderbliss::Epsilon());
    CHECK_CLOSE(y, colour.y, renderbliss::Epsilon());
    CHECK_CLOSE(z, colour.z, renderbliss::Epsilon());
}

TEST_FIXTURE(XYZFixture, CheckToRGB)
{
    renderbliss::RGB colourRGB = colour.ToRGB();
    renderbliss::XYZ colourXYZ = colourRGB.ToXYZ();
    CHECK_CLOSE(colour.x, colourXYZ.x, 0.001f);
    CHECK_CLOSE(colour.y, colourXYZ.y, 0.001f);
    CHECK_CLOSE(colour.z, colourXYZ.z, 0.001f);
}

TEST_FIXTURE(XYZFixture, CheckOperatorIndex)
{
    CHECK_CLOSE(colour[0], colour.x, renderbliss::Epsilon());
    CHECK_CLOSE(colour[1], colour.y, renderbliss::Epsilon());
    CHECK_CLOSE(colour[2], colour.z, renderbliss::Epsilon());
}

TEST_FIXTURE(XYZFixture, CheckOperatorNegation)
{
    CHECK_CLOSE((-colour).x, -x, renderbliss::Epsilon());
    CHECK_CLOSE((-colour).y, -y, renderbliss::Epsilon());
    CHECK_CLOSE((-colour).z, -z, renderbliss::Epsilon());
}

TEST_FIXTURE(XYZFixture, CheckOperatorPlusEqual)
{
    colour += renderbliss::XYZ(1,2,3);
    CHECK_CLOSE(colour.x, x+1, renderbliss::Epsilon());
    CHECK_CLOSE(colour.y, y+2, renderbliss::Epsilon());
    CHECK_CLOSE(colour.z, z+3, renderbliss::Epsilon());
}

TEST_FIXTURE(XYZFixture, CheckOperatorMinusEqual)
{
    colour -= renderbliss::XYZ(1,2,3);
    CHECK_CLOSE(colour.x, x-1, renderbliss::Epsilon());
    CHECK_CLOSE(colour.y, y-2, renderbliss::Epsilon());
    CHECK_CLOSE(colour.z, z-3, renderbliss::Epsilon());
}

TEST_FIXTURE(XYZFixture, CheckOperatorTimesEqual)
{
    colour *= 4.0f;
    CHECK_CLOSE(colour.x, x*4, renderbliss::Epsilon());
    CHECK_CLOSE(colour.y, y*4, renderbliss::Epsilon());
    CHECK_CLOSE(colour.z, z*4, renderbliss::Epsilon());
}

TEST_FIXTURE(XYZFixture, CheckOperatorDivEqual)
{
    colour /= 4.0f;
    CHECK_CLOSE(colour.x, x/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(colour.y, y/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(colour.z, z/4.0f, renderbliss::Epsilon());
}
}
