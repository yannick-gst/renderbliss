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
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Utils/AtomicOps.h"

namespace
{
using namespace renderbliss;
TEST(CheckAtomicAddUint32)
{
    uint32 i = 1;
    AtomicAdd(&i, 1);
    CHECK_EQUAL(static_cast<uint32>(2), i);
    AtomicAdd(&i, 4);
    CHECK_EQUAL(static_cast<uint32>(6), i);
}

TEST(CheckAtomicAddFloat)
{
    float  f = 1.5f;
    AtomicAdd(&f, 0.5f);
    CHECK_CLOSE(2.0f, f, Epsilon());
    AtomicAdd(&f, 4.5f);
    CHECK_CLOSE(6.5f, f, Epsilon());
    AtomicAdd(&f, -5.0f);
    CHECK_CLOSE(1.5f, f, Epsilon());
}

TEST(CheckAtomicIncrementUint32)
{
    uint32 i = 1;
    AtomicIncrement(&i);
    CHECK_EQUAL(static_cast<uint32>(2), i);
}

TEST(CheckAtomicCounter)
{
    AtomicCounter c;
    ++c;
    CHECK_EQUAL(static_cast<uint32>(1), c);
    c++;
    CHECK_EQUAL(static_cast<uint32>(2), c);
    uint32 i = c;
    CHECK_EQUAL(i, c);
}
}
