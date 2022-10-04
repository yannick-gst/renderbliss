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
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Vector2.h"
#include "renderbliss/Math/Geometry/Vector3.h"

namespace
{
struct MersenneTwisterFixture
{
    renderbliss::MersenneTwister mt;
};

TEST_FIXTURE(MersenneTwisterFixture, CheckCanonicalRandom)
{
    renderbliss::real r = mt.CanonicalRandom();
    CHECK((r >= 0) && (r < static_cast<renderbliss::real>(1.0f)));

    renderbliss::Vector2 r2 = mt.CanonicalRandom2();
    CHECK((r2.x >= 0) && (r2.x < static_cast<renderbliss::real>(1.0f)));
    CHECK((r2.y >= 0) && (r2.y < static_cast<renderbliss::real>(1.0f)));

    renderbliss::Vector3 r3 = mt.CanonicalRandom3();
    CHECK((r3.x >= 0) && (r3.x < static_cast<renderbliss::real>(1.0f)));
    CHECK((r3.y >= 0) && (r3.y < static_cast<renderbliss::real>(1.0f)));
    CHECK((r3.z >= 0) && (r3.z < static_cast<renderbliss::real>(1.0f)));
}

TEST_FIXTURE(MersenneTwisterFixture, CheckRandomReal)
{
    renderbliss::real r = mt.RandomReal(15.0f, 235.0f);
    CHECK(r >= static_cast<renderbliss::real>(15.0f));
    CHECK(r < static_cast<renderbliss::real>(235.0f));
}

TEST_FIXTURE(MersenneTwisterFixture, CheckRandomUint)
{
    renderbliss::uint r = mt.RandomUint(static_cast<renderbliss::uint>(235));
    CHECK(r <= static_cast<renderbliss::uint>(235));

    r = mt.RandomUint(static_cast<renderbliss::uint>(15), static_cast<renderbliss::uint>(235));
    CHECK(r >= static_cast<renderbliss::uint>(15));
    CHECK(r <= static_cast<renderbliss::uint>(235));
}
}
