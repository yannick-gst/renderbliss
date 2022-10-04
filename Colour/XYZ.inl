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

#include "renderbliss/Macros.h"

namespace renderbliss
{
inline XYZ::XYZ(float val) : x(val), y(val), z(val)
{}

inline XYZ::XYZ(const float arr[3]) : x(arr[0]), y(arr[1]), z(arr[2])
{}

inline XYZ::XYZ(float x, float y, float z) : x(x), y(y), z(z)
{}

inline float XYZ::operator[](unsigned i) const
{
    RB_ASSERT(i < 3);
    return *(&x + i);
}

inline float& XYZ::operator[](unsigned i)
{
    RB_ASSERT(i < 3);
    return *(&x + i);
}

inline XYZ XYZ::operator-() const
{
    return XYZ(-x, -y, -z);
}

inline XYZ& XYZ::operator=(const XYZ& colour)
{
    x = colour.x;
    y = colour.y;
    z = colour.z;
    return *this;
}

inline XYZ& XYZ::operator+=(const XYZ& colour)
{
    x += colour.x;
    y += colour.y;
    z += colour.z;
    return *this;
}

inline XYZ& XYZ::operator-=(const XYZ& colour)
{
    x -= colour.x;
    y -= colour.y;
    z -= colour.z;
    return *this;
}

inline XYZ& XYZ::operator*=(const XYZ& colour)
{
    x *= colour.x;
    y *= colour.y;
    z *= colour.z;
    return *this;
}

inline XYZ& XYZ::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

inline XYZ& XYZ::operator/=(const XYZ& colour)
{
    RB_ASSERT(colour.x != 0.0f);
    RB_ASSERT(colour.y != 0.0f);
    RB_ASSERT(colour.z != 0.0f);
    x /= colour.x;
    y /= colour.y;
    z /= colour.z;
    return *this;
}

inline XYZ& XYZ::operator/=(float scalar)
{
    RB_ASSERT(scalar != 0.0f);
    scalar = 1.0f/scalar;
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

inline bool operator==(const XYZ& lhs, const XYZ& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

inline bool operator!=(const XYZ& lhs, const XYZ& rhs)
{
    return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z);
}

inline XYZ operator+(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs) += rhs;
}

inline XYZ operator-(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs) -= rhs;
}

inline XYZ operator*(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs) *= rhs;
}

inline XYZ operator*(float scalar, const XYZ& colour)
{
    return XYZ(colour) *= scalar;
}

inline XYZ operator*(const XYZ& colour, float scalar)
{
    return XYZ(colour) *= scalar;
}

inline XYZ operator/(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs) /= rhs;
}

inline XYZ operator/(const XYZ& colour, float scalar)
{
    return XYZ(colour) /= scalar;
}
}
