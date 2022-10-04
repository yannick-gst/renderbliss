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

#include "renderbliss/Math/MersenneTwister.h"
#include <algorithm>
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Vector2.h"
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
MersenneTwister::MersenneTwister(uint seed)
    : realGenerator(boost::mt19937(seed))
{}

real MersenneTwister::CanonicalRandom()
{
    return realGenerator();
}

Vector2 MersenneTwister::CanonicalRandom2()
{
    return Vector2(realGenerator(), realGenerator());
}

Vector3 MersenneTwister::CanonicalRandom3()
{
    return Vector3(realGenerator(), realGenerator(), realGenerator());
}

real MersenneTwister::RandomReal(real min, real max)
{
    if (min==max) return min;
    if (min > max) std::swap(min, max);
    return Lerp(min, max, realGenerator());
}

uint MersenneTwister::RandomUint(uint max)
{
    if (max==0) return 0;
    return static_cast<uint>(RandomReal(static_cast<real>(0), static_cast<real>(max+1)));
}

uint MersenneTwister::RandomUint(uint min, uint max)
{
    if (min==max) return min;
    min = std::min(min, max);
    max = std::max(min, max) + 1;
    return static_cast<uint>(RandomReal(static_cast<real>(min), static_cast<real>(max)));
}
}
