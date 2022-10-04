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

#include "renderbliss/Colour/XYZ.h"
#include <ostream>
#include "renderbliss/Colour/RGB.h"

namespace renderbliss
{
XYZ::XYZ(const RGB& rgb)
{
    XYZ xyz(rgb.ToXYZ());
    x=xyz.x, y=xyz.y, z=xyz.z;
}

RGB XYZ::ToRGB() const
{
    static const float XYZToRGB[3][3] =
    {
        { 3.240479f, -1.537150f, -0.498535f},
        {-0.969256f,  1.875992f,  0.041556f},
        { 0.055648f, -0.204043f,  1.057311f}
    };

    return RGB( x*XYZToRGB[0][0] + y*XYZToRGB[0][1] + z*XYZToRGB[0][2],
        x*XYZToRGB[1][0] + y*XYZToRGB[1][1] + z*XYZToRGB[1][2],
        x*XYZToRGB[2][0] + y*XYZToRGB[2][1] + z*XYZToRGB[2][2]);
}

std::ostream& operator<<(std::ostream& os, const XYZ& colour)
{
    return os   << "("
        << colour.x << ", "
        << colour.y << ", "
        << colour.z
        << ")";
}
}
