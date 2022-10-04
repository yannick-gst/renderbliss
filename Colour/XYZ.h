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

#ifndef RENDERBLISS_XYZ_H
#define RENDERBLISS_XYZ_H

#include <iosfwd>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct RGB;

struct XYZ
{
    float x, y, z;

    XYZ(float val = 0.0f);
    XYZ(const RGB& rgb);
    XYZ(const float arr[3]);
    XYZ(float x, float y, float z);

    RGB  ToRGB() const;

    float& operator[](unsigned i);
    float  operator[](unsigned i) const;
    XYZ   operator -() const;
    XYZ&  operator =(const XYZ& colour);
    XYZ&  operator+=(const XYZ& colour);
    XYZ&  operator-=(const XYZ& colour);
    XYZ&  operator*=(const XYZ& colour);
    XYZ&  operator*=(float scalar);
    XYZ&  operator/=(const XYZ& colour);
    XYZ&  operator/=(float scalar);

};

bool operator==(const XYZ& lhs, const XYZ& rhs);
bool operator!=(const XYZ& lhs, const XYZ& rhs);
XYZ  operator +(const XYZ& lhs, const XYZ& rhs);
XYZ  operator -(const XYZ& lhs, const XYZ& rhs);
XYZ  operator *(const XYZ& lhs, const XYZ& rhs);
XYZ  operator *(float scalar, const XYZ& colour);
XYZ  operator *(const XYZ& colour, float scalar);
XYZ  operator /(const XYZ& lhs, const XYZ& rhs);
XYZ  operator /(const XYZ& colour, float scalar);

std::ostream& operator<<(std::ostream& os, const XYZ& colour);
}

#include "renderbliss/Colour/XYZ.inl"

#endif
