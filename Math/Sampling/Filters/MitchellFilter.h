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

#ifndef RENDERBLISS_MITCHELL_FILTER_H
#define RENDERBLISS_MITCHELL_FILTER_H

#include "renderbliss/Interfaces/IFilter.h"

namespace renderbliss
{
// Reference:
// "Reconstruction filters in computer graphics"
// Don. P. Mitchell, Arun N. Netravali
// ACM SIGGRAPH Computer Graphics, Vol. 22, Number 4; August 1988
class MitchellFilter : public IFilter
{
public:

    MitchellFilter(real xWidth=1.0f, real yWidth=1.0f, real B=0.0f, real C=0.5f); // It is recommended that B + 2C = 1
    virtual real Weight(real x, real y) const;

private:

    real dx, dy;
    real a3_1, a2_1, a0_1, a3_2, a2_2, a1_2, a0_2; // Coefficients of K(x)
    real K(real x) const;
};

MitchellFilter CreateCubicFilter(real xWidth, real yWidth);
MitchellFilter CreateCatmullRomFilter(real xWidth, real yWidth);
}

#endif
