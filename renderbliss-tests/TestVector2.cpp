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
#include <boost/random.hpp>
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Vector2.h"

namespace
{
boost::mt19937 rng;
boost::uniform_real<renderbliss::real> dist(-225, 225);
boost::variate_generator<boost::mt19937, boost::uniform_real<renderbliss::real> > die(rng, dist);

struct Vector2Fixture
{
    renderbliss::real x, y;
    renderbliss::Vector2 vec;
    Vector2Fixture() : x(die()), y(die()), vec(x,y) {}
};

TEST(CheckUnitX)
{
    CHECK_CLOSE(1, renderbliss::Vector2::unitX.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector2::unitX.y, renderbliss::Epsilon());
}

TEST(CheckUnitY)
{
    CHECK_CLOSE(0, renderbliss::Vector2::unitY.x, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::Vector2::unitY.y, renderbliss::Epsilon());
}

TEST(CheckZero)
{
    CHECK_CLOSE(0, renderbliss::Vector2::zero.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector2::zero.y, renderbliss::Epsilon());
}

TEST(CheckConstructorFromSingleRealParameter)
{
    renderbliss::real val = die();
    renderbliss::Vector2 vec(val);

    CHECK_CLOSE(val, vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(val, vec.y, renderbliss::Epsilon());
}

TEST(CheckConstructorFromArrayParameter)
{
    renderbliss::real arr[2] =  { die(), die() };
    renderbliss::Vector2 vec(arr);

    CHECK_CLOSE(arr[0], vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(arr[1], vec.y, renderbliss::Epsilon());
}

TEST(CheckConstructorFromTwoRealParameters)
{
    renderbliss::real x=die(), y=die();
    renderbliss::Vector2 vec(x, y);

    CHECK_CLOSE(x, vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(y, vec.y, renderbliss::Epsilon());
}

TEST(CheckNorm)
{
    renderbliss::Vector2 vec(20, 15);
    renderbliss::real expected = 25;
    CHECK_CLOSE(expected, vec.Norm(), renderbliss::Epsilon());
}

TEST(CheckSquaredNorm)
{
    renderbliss::Vector2 vec(15, 20);
    renderbliss::real expected = 625;
    CHECK_CLOSE(expected, vec.SquaredNorm(), renderbliss::Epsilon());
}

TEST(CheckNormalize)
{
    renderbliss::Vector2 vec(die());
    vec.Normalize();
    CHECK_CLOSE(1, vec.Norm(), renderbliss::Epsilon());
}

TEST(CheckGetNormalized)
{
    renderbliss::Vector2 vec(die());
    renderbliss::Vector2 normalisedVec = vec.GetNormalized();
    CHECK_CLOSE(1, normalisedVec.Norm(), renderbliss::Epsilon());
}

TEST(CheckDistance)
{
    renderbliss::Vector2 a(-1.0f, -1.0f);
    renderbliss::Vector2 b( 1.0f,  1.0f);
    renderbliss::real expected = sqrt(static_cast<renderbliss::real>(8.0f));
    renderbliss::real distance = Distance(a, b);
    CHECK_CLOSE(expected, distance, renderbliss::Epsilon());
}

TEST(CheckSquaredDistance)
{
    renderbliss::Vector2 a(-1.0f, -1.0f);
    renderbliss::Vector2 b( 1.0f,  1.0f);
    renderbliss::real expected = 8.0f;
    renderbliss::real sqrDistance = SquaredDistance(a, b);
    CHECK_CLOSE(expected, sqrDistance, renderbliss::Epsilon());
}

TEST(CheckDotProduct)
{
    renderbliss::Vector2 vec(-1,1);
    renderbliss::real dot = renderbliss::DotProduct(vec, renderbliss::Vector2::unitX);
    CHECK_CLOSE(-1, dot, renderbliss::Epsilon());
}

TEST(CheckAbsDotProduct)
{
    renderbliss::Vector2 vec(-1,1);
    renderbliss::real absDot = renderbliss::AbsDotProduct(vec, renderbliss::Vector2::unitX);
    CHECK_CLOSE(1, absDot, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector2Fixture, CheckOperatorIndex)
{
    CHECK_CLOSE(vec[0], vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(vec[1], vec.y, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector2Fixture, CheckOperatorNegation)
{
    CHECK_CLOSE((-vec).x, -x, renderbliss::Epsilon());
    CHECK_CLOSE((-vec).y, -y, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector2Fixture, CheckOperatorPlusEqual)
{
    vec += renderbliss::Vector2(1,2);
    CHECK_CLOSE(vec.x, x+1, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y+2, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector2Fixture, CheckOperatorMinusEqual)
{
    vec -= renderbliss::Vector2(1,2);
    CHECK_CLOSE(vec.x, x-1, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y-2, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector2Fixture, CheckOperatorTimesEqual)
{
    vec *= 4.0f;
    CHECK_CLOSE(vec.x, x*4, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y*4, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector2Fixture, CheckOperatorDivEqual)
{
    vec /= 4.0f;
    CHECK_CLOSE(vec.x, x/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y/4.0f, renderbliss::Epsilon());
}
}
