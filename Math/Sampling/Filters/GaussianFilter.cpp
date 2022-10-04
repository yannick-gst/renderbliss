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

#include "renderbliss/Math/Sampling/Filters/GaussianFilter.h"
#include <algorithm>
#include <cmath>
#include "renderbliss/Math/Sampling/Filters/ExtentsCheckMacro.h"

namespace
{
using namespace renderbliss;
real Gaussian(real x, real fallOffRate, real expWidth)
{
    return std::max(static_cast<real>(0.0f), std::exp(-fallOffRate*x*x) - expWidth);
}
}

namespace renderbliss
{
GaussianFilter::GaussianFilter(real xWidth, real yWidth, real fallOffRate)
    :   IFilter(xWidth, yWidth),
        fallOffRate(fallOffRate),
        expX(std::exp(-fallOffRate*xWidth*xWidth)),
        expY(std::exp(-fallOffRate*yWidth*yWidth))
{
}

real GaussianFilter::Weight(real x, real y) const
{
    CHECK_AGAINST_FILTER_EXTENTS(x, y);
    return Gaussian(x, fallOffRate, expX) * Gaussian(y, fallOffRate, expY);
}
}
