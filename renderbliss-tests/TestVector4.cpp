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
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Vector4.h"

namespace
{
boost::mt19937 rng;
boost::uniform_real<renderbliss::real> dist(-225, 225);
boost::variate_generator<boost::mt19937&, boost::uniform_real<renderbliss::real> > die(rng, dist);

struct Vector4Fixture
{
    renderbliss::real x, y, z, w;
    renderbliss::Vector4 vec;
    Vector4Fixture() : x(die()), y(die()), z(die()), w(die()), vec(x,y,z,w) {}
};

TEST(CheckUnitX)
{
    CHECK_CLOSE(1, renderbliss::Vector4::unitX.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitX.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitX.z, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitX.w, renderbliss::Epsilon());
}

TEST(CheckUnitY)
{
    CHECK_CLOSE(0, renderbliss::Vector4::unitY.x, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::Vector4::unitY.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitY.z, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitY.w, renderbliss::Epsilon());
}

TEST(CheckUnitZ)
{
    CHECK_CLOSE(0, renderbliss::Vector4::unitZ.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitZ.y, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::Vector4::unitZ.z, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitZ.w, renderbliss::Epsilon());
}

TEST(CheckUnitW)
{
    CHECK_CLOSE(0, renderbliss::Vector4::unitW.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitW.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::unitW.z, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::Vector4::unitW.w, renderbliss::Epsilon());
}

TEST(CheckZero)
{
    CHECK_CLOSE(0, renderbliss::Vector4::zero.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::zero.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::zero.z, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector4::zero.w, renderbliss::Epsilon());
}

TEST(CheckConstructorFromSingleRealParameter)
{
    renderbliss::real val = die();
    renderbliss::Vector4 vec(val);

    CHECK_CLOSE(val, vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(val, vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(val, vec.z, renderbliss::Epsilon());
    CHECK_CLOSE(val, vec.w, renderbliss::Epsilon());
}

TEST(CheckConstructorFromArrayParameter)
{
    renderbliss::real arr[4] =  { die(), die(), die(), die() };
    renderbliss::Vector4 vec(arr);

    CHECK_CLOSE(arr[0], vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(arr[1], vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(arr[2], vec.z, renderbliss::Epsilon());
    CHECK_CLOSE(arr[3], vec.w, renderbliss::Epsilon());
}

TEST(CheckConstructorFromVector3Parameter)
{
    renderbliss::Vector3 vec3(die(), die(), die());
    renderbliss::Vector4 vec4(vec3);

    CHECK_CLOSE(vec3.x, vec4.x, renderbliss::Epsilon());
    CHECK_CLOSE(vec3.y, vec4.y, renderbliss::Epsilon());
    CHECK_CLOSE(vec3.z, vec4.z, renderbliss::Epsilon());
    CHECK_CLOSE(1, vec4.w, renderbliss::Epsilon());
}

TEST(CheckConstructorFromFourRealParameters)
{
    renderbliss::real x=die(), y=die(), z=die(), w=die();
    renderbliss::Vector4 vec(x, y, z, w);

    CHECK_CLOSE(x, vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(y, vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(z, vec.z, renderbliss::Epsilon());
    CHECK_CLOSE(w, vec.w, renderbliss::Epsilon());
}

TEST(CheckNorm)
{
    renderbliss::Vector4 vec(15, 15, 15, 15);
    renderbliss::real expected = 30;
    CHECK_CLOSE(expected, vec.Norm(), renderbliss::Epsilon());
}

TEST(CheckSquaredNorm)
{
    renderbliss::Vector4 vec(15, 15, 15, 15);
    renderbliss::real expected = 900;
    CHECK_CLOSE(expected, vec.SquaredNorm(), renderbliss::Epsilon());
}

TEST(CheckNormalize)
{
    renderbliss::Vector4 vec(die());
    vec.Normalize();
    CHECK_CLOSE(1, vec.Norm(), renderbliss::Epsilon());
}

TEST(CheckNormalized)
{
    renderbliss::Vector4 vec(die());
    renderbliss::Vector4 normalisedVec = vec.Normalized();
    CHECK_CLOSE(1, normalisedVec.Norm(), renderbliss::Epsilon());
}

TEST_FIXTURE(Vector4Fixture, CheckOperatorIndex)
{
    CHECK_CLOSE(vec[0], vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(vec[1], vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(vec[2], vec.z, renderbliss::Epsilon());
    CHECK_CLOSE(vec[3], vec.w, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector4Fixture, CheckOperatorNegation)
{
    CHECK_CLOSE((-vec).x, -x, renderbliss::Epsilon());
    CHECK_CLOSE((-vec).y, -y, renderbliss::Epsilon());
    CHECK_CLOSE((-vec).z, -z, renderbliss::Epsilon());
    CHECK_CLOSE((-vec).w, -w, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector4Fixture, CheckOperatorPlusEqual)
{
    vec += renderbliss::Vector4(1,2,3,4);
    CHECK_CLOSE(vec.x, x+1, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y+2, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z+3, renderbliss::Epsilon());
    CHECK_CLOSE(vec.w, w+4, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector4Fixture, CheckOperatorMinusEqual)
{
    vec -= renderbliss::Vector4(1,2,3,4);
    CHECK_CLOSE(vec.x, x-1, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y-2, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z-3, renderbliss::Epsilon());
    CHECK_CLOSE(vec.w, w-4, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector4Fixture, CheckOperatorTimesEqual)
{
    vec *= 4.0f;
    CHECK_CLOSE(vec.x, x*4, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y*4, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z*4, renderbliss::Epsilon());
    CHECK_CLOSE(vec.w, w*4, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector4Fixture, CheckOperatorDivEqual)
{
    vec /= 4.0f;
    CHECK_CLOSE(vec.x, x/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(vec.w, w/4.0f, renderbliss::Epsilon());
}
}
