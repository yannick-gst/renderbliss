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

#include "renderbliss/Colour/RGB.h"
#include <ostream>
#include "renderbliss/Colour/XYZ.h"

namespace renderbliss
{
const RGB RGB::black(0.0f);
const RGB RGB::white(1.0f);

RGB::RGB(const XYZ& xyz)
{
    RGB rgb(xyz.ToRGB());
    r=rgb.r, g=rgb.g, b=rgb.b;
}

XYZ RGB::ToXYZ() const
{
    static const float RGBToXYZ[3][3] = 
    {
        {0.412453f, 0.357580f, 0.180423f},
        {0.212671f, 0.715160f, 0.072169f},
        {0.019334f, 0.119193f, 0.950227f}
    };

    return XYZ( r*RGBToXYZ[0][0] + g*RGBToXYZ[0][1] + b*RGBToXYZ[0][2],
        r*RGBToXYZ[1][0] + g*RGBToXYZ[1][1] + b*RGBToXYZ[1][2],
        r*RGBToXYZ[2][0] + g*RGBToXYZ[2][1] + b*RGBToXYZ[2][2]);
}

std::ostream& operator<<(std::ostream& os, const RGB& colour)
{
    return os   << "("
        << colour.r << ", "
        << colour.g << ", "
        << colour.b
        << ")";
}
}
