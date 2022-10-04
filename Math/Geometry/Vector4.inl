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
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
inline Vector4::Vector4()
{}

inline Vector4::Vector4(real val) : x(val), y(val), z(val), w(val)
{}

inline Vector4::Vector4(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z), w(1.0f)
{}

inline Vector4::Vector4(const real arr[4]) :x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])
{}

inline Vector4::Vector4(real x, real y, real z, real w) : x(x), y(y), z(z), w(w)
{}

inline void Vector4::Normalize()
{
    *this /= Norm();
}

inline real Vector4::Norm() const
{
    return sqrt(SquaredNorm());
}

inline real Vector4::SquaredNorm() const
{
    return x*x + y*y + z*z + w*w;
}

inline Vector4 Vector4::Normalized() const
{
    Vector4 temp(*this);
    temp.Normalize();
    return temp;
}

inline real& Vector4::operator[](unsigned i)
{
    RB_ASSERT(i<4);
    return *(&x + i);
}

inline real Vector4::operator[](unsigned i) const
{
    RB_ASSERT(i<4);
    return *(&x + i);
}

inline Vector4 Vector4::operator-() const
{
    return Vector4(-x, -y, -z, -w);
}

inline Vector4& Vector4::operator=(const Vector4& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;
    return *this;
}

inline Vector4& Vector4::operator+=(const Vector4& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;
    return *this;
}

inline Vector4& Vector4::operator-=(const Vector4& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;
    return *this;
}

inline Vector4& Vector4::operator*=(real scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

inline Vector4& Vector4::operator/=(real scalar)
{
    RB_ASSERT(scalar != 0.0f);
    return *this *= (1.0f/scalar);
}

inline bool operator==(const Vector4& lhs, const Vector4& rhs)
{
    return (lhs.x==rhs.x) && (lhs.y==rhs.y) && (lhs.z==rhs.z) && (lhs.w==rhs.w);
}

inline bool operator!=(const Vector4& lhs, const Vector4& rhs)
{
    return (lhs.x!=rhs.x) || (lhs.y!=rhs.y) || (lhs.z!=rhs.z) || (lhs.w!=rhs.w);
}

inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
{
    return Vector4(lhs) += rhs;
}

inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
{
    return Vector4(lhs) -= rhs;
}

inline Vector4 operator*(real scalar, const Vector4& vec)
{
    return Vector4(vec) *= scalar;
}

inline Vector4 operator*(const Vector4& vec, real scalar)
{
    return Vector4(vec) *= scalar;
}

inline Vector4 operator/(const Vector4& vec, real scalar)
{
    return Vector4(vec) /= scalar;
}
}
