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

#include "renderbliss/Math/Sampling/Filters/MitchellFilter.h"
#include "renderbliss/Math/Sampling/Filters/ExtentsCheckMacro.h"

namespace renderbliss
{
MitchellFilter::MitchellFilter(real xWidth, real yWidth, real B, real C)
    :   IFilter(xWidth, yWidth),
        a3_1(12.f - 9.f*B - 6.f*C),
        a2_1(-18.f + 12.f*B + 6.f*C),
        a0_1(6.f - 2.f*B),
        a3_2(-B - 6.f*C),
        a2_2(6.f*B + 30.f*C),
        a1_2(-12.f*B - 48.f*C),
        a0_2(8.f*B + 24.f*C)
{
}

real MitchellFilter::K(real x) const
{
    static const real inv6 = 1.0f/6.0f;
    x = fabs(2*x);
    real x2 = x*x;
    return  (x < 1) ? (a3_1*x2*x + a2_1*x2 + a0_1)*inv6
        :   (x < 2) ? (a3_2*x2*x + a2_2*x2 + a1_2*x + a0_2)*inv6
        :   0;
}

real MitchellFilter::Weight(real x, real y) const
{
    CHECK_AGAINST_FILTER_EXTENTS(x, y);
    return K(x * Dx()) * K(y * Dy());
}

MitchellFilter CreateCubicFilter(real xWidth, real yWidth)
{
    return MitchellFilter(xWidth, yWidth, 1, 0);
}

MitchellFilter CreateCatmullRomFilter(real xWidth, real yWidth)
{
    return MitchellFilter(xWidth, yWidth, 0, 0.5f);
}
}
