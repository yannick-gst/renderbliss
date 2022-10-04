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

#include "renderbliss/Math/Sampling/Filters/LanczosFilter.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Sampling/Filters/ExtentsCheckMacro.h"

namespace renderbliss
{
LanczosFilter::LanczosFilter(real xWidth, real yWidth, real windowWidth)
    : IFilter(xWidth, yWidth), windowWidth(windowWidth)
{
    RB_ASSERT(windowWidth > 0);
}

real LanczosFilter::Weight(real x, real y) const
{
    CHECK_AGAINST_FILTER_EXTENTS(x, y);
    return WindowedSinc(x) * WindowedSinc(y);
}

real LanczosFilter::WindowedSinc(real x) const
{
    x = fabs(x);

    if (x < 1e-5f)
    {
        return 1.f;
    }

    x *= Pi();
    return Sinc(x*windowWidth) * Sinc(x);
}
}
