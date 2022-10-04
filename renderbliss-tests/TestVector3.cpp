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
#include "renderbliss/Math/Geometry/Vector3.h"

namespace
{
boost::mt19937 rng;
boost::uniform_real<renderbliss::real> dist(-225, 225);
boost::variate_generator<boost::mt19937, boost::uniform_real<renderbliss::real> > die(rng, dist);

struct Vector3Fixture
{
    renderbliss::real x, y, z;
    renderbliss::Vector3 vec;
    Vector3Fixture() : x(die()), y(die()), z(die()), vec(x,y,z) {}
};

TEST(CheckUnitX)
{
    CHECK_CLOSE(1, renderbliss::Vector3::unitX.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector3::unitX.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector3::unitX.z, renderbliss::Epsilon());
}

TEST(CheckUnitY)
{
    CHECK_CLOSE(0, renderbliss::Vector3::unitY.x, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::Vector3::unitY.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector3::unitY.z, renderbliss::Epsilon());
}

TEST(CheckUnitZ)
{
    CHECK_CLOSE(0, renderbliss::Vector3::unitZ.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector3::unitZ.y, renderbliss::Epsilon());
    CHECK_CLOSE(1, renderbliss::Vector3::unitZ.z, renderbliss::Epsilon());
}

TEST(CheckZero)
{
    CHECK_CLOSE(0, renderbliss::Vector3::zero.x, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector3::zero.y, renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::Vector3::zero.z, renderbliss::Epsilon());
}

TEST(CheckConstructorFromSingleRealParameter)
{
    renderbliss::real val = die();
    renderbliss::Vector3 vec(val);

    CHECK_CLOSE(val, vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(val, vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(val, vec.z, renderbliss::Epsilon());
}

TEST(CheckConstructorFromArrayParameter)
{
    renderbliss::real arr[3] =  { die(), die(), die() };
    renderbliss::Vector3 vec(arr);

    CHECK_CLOSE(arr[0], vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(arr[1], vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(arr[2], vec.z, renderbliss::Epsilon());
}

TEST(CheckConstructorFromThreeRealParameters)
{
    renderbliss::real x=die(), y=die(), z=die();
    renderbliss::Vector3 vec(x, y, z);

    CHECK_CLOSE(x, vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(y, vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(z, vec.z, renderbliss::Epsilon());
}

TEST(CheckNorm)
{
    renderbliss::Vector3 vec(10, 20, 20);
    renderbliss::real expected = 30;
    CHECK_CLOSE(expected, vec.Norm(), renderbliss::Epsilon());
}

TEST(CheckSquaredNorm)
{
    renderbliss::Vector3 vec(10, 20, 20);
    renderbliss::real expected = 900;
    CHECK_CLOSE(expected, vec.SquaredNorm(), renderbliss::Epsilon());
}

TEST(CheckNormalize)
{
    renderbliss::Vector3 vec(die());
    vec.Normalize();
    CHECK_CLOSE(1, vec.Norm(), renderbliss::Epsilon());
}

TEST(CheckGetNormalized)
{
    renderbliss::Vector3 vec(die());
    renderbliss::Vector3 normalisedVec = vec.GetNormalized();
    CHECK_CLOSE(1, normalisedVec.Norm(), renderbliss::Epsilon());
}

TEST(CheckDistance)
{
    renderbliss::Vector3 a(-1.0f, -1.0f, 1.0f);
    renderbliss::Vector3 b( 1.0f,  1.0f, -1.0f);
    renderbliss::real expected = sqrt(static_cast<renderbliss::real>(12.0f));
    renderbliss::real distance = Distance(a, b);
    CHECK_CLOSE(expected, distance, renderbliss::Epsilon());
}

TEST(CheckSquaredDistance)
{
    renderbliss::Vector3 vec1(-1.0f, -1.0f, 1.0f);
    renderbliss::Vector3 vec2( 1.0f,  1.0f, -1.0f);
    renderbliss::real expected = 12.0f;
    renderbliss::real sqrDistance = SquaredDistance(vec1, vec2);
    CHECK_CLOSE(expected, sqrDistance, renderbliss::Epsilon());
}

TEST(CheckDotProduct)
{
    renderbliss::Vector3 vec(-1,1,1);
    renderbliss::real dot = renderbliss::DotProduct(vec, renderbliss::Vector3::unitX);
    CHECK_CLOSE(-1, dot, renderbliss::Epsilon());
}

TEST(CheckAbsDotProduct)
{
    renderbliss::Vector3 vec(-1,1,1);
    renderbliss::real absDot = renderbliss::AbsDotProduct(vec, renderbliss::Vector3::unitX);
    CHECK_CLOSE(1, absDot, renderbliss::Epsilon());
}

TEST(CheckCrossProduct)
{
    renderbliss::Vector3 cross = renderbliss::CrossProduct(renderbliss::Vector3::unitX, renderbliss::Vector3::unitY);
    CHECK_CLOSE(cross.x, renderbliss::Vector3::unitZ.x, renderbliss::Epsilon());
    CHECK_CLOSE(cross.y, renderbliss::Vector3::unitZ.y, renderbliss::Epsilon());
    CHECK_CLOSE(cross.z, renderbliss::Vector3::unitZ.z, renderbliss::Epsilon());
}

TEST(CheckSphericalToCartesianFromAngles)
{
    renderbliss::real phi = renderbliss::FourthPi();
    renderbliss::real theta = renderbliss::FourthPi();
    renderbliss::real r = 3.0f;

    renderbliss::Vector3 expected(1.5f, 1.5f, 1.5f*renderbliss::Sqrt2());
    renderbliss::Vector3 actual = renderbliss::SphericalToCartesian(phi, theta, r);

    CHECK_CLOSE(expected.x, actual.x, renderbliss::Epsilon());
    CHECK_CLOSE(expected.y, actual.y, renderbliss::Epsilon());
    CHECK_CLOSE(expected.z, actual.z, renderbliss::Epsilon());
}

TEST(CheckSphericalToCartesianFromTrigonometricValues)
{
    renderbliss::real cosPhi = 0.5f*renderbliss::Sqrt2();
    renderbliss::real cosTheta = 0.5f*renderbliss::Sqrt2();
    renderbliss::real sinPhi = 0.5f*renderbliss::Sqrt2();
    renderbliss::real sinTheta = 0.5f*renderbliss::Sqrt2();
    renderbliss::real r = 3.0f;

    renderbliss::Vector3 expected(1.5f, 1.5f, 1.5f*renderbliss::Sqrt2());
    renderbliss::Vector3 actual = renderbliss::SphericalToCartesian(cosPhi, sinPhi, cosTheta, sinTheta, r);

    CHECK_CLOSE(expected.x, actual.x, renderbliss::Epsilon());
    CHECK_CLOSE(expected.y, actual.y, renderbliss::Epsilon());
    CHECK_CLOSE(expected.z, actual.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector3Fixture, CheckOperatorIndex)
{
    CHECK_CLOSE(vec[0], vec.x, renderbliss::Epsilon());
    CHECK_CLOSE(vec[1], vec.y, renderbliss::Epsilon());
    CHECK_CLOSE(vec[2], vec.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector3Fixture, CheckOperatorNegation)
{
    CHECK_CLOSE((-vec).x, -x, renderbliss::Epsilon());
    CHECK_CLOSE((-vec).y, -y, renderbliss::Epsilon());
    CHECK_CLOSE((-vec).z, -z, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector3Fixture, CheckOperatorPlusEqual)
{
    vec += renderbliss::Vector3(1,2,3);
    CHECK_CLOSE(vec.x, x+1, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y+2, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z+3, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector3Fixture, CheckOperatorMinusEqual)
{
    vec -= renderbliss::Vector3(1,2,3);
    CHECK_CLOSE(vec.x, x-1, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y-2, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z-3, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector3Fixture, CheckOperatorTimesEqual)
{
    vec *= 4.0f;
    CHECK_CLOSE(vec.x, x*4, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y*4, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z*4, renderbliss::Epsilon());
}

TEST_FIXTURE(Vector3Fixture, CheckOperatorDivEqual)
{
    vec /= 4.0f;
    CHECK_CLOSE(vec.x, x/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(vec.y, y/4.0f, renderbliss::Epsilon());
    CHECK_CLOSE(vec.z, z/4.0f, renderbliss::Epsilon());
}
}
