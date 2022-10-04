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
#include "renderbliss/Math/MathUtils.h"

namespace renderbliss
{
inline Vector2::Vector2()
{}

inline Vector2::Vector2(real val) : x(val), y(val)
{}

inline Vector2::Vector2(const real arr[2]) :x(arr[0]), y(arr[1])
{}

inline Vector2::Vector2(real x, real y) : x(x), y(y)
{}

inline void Vector2::Normalize()
{
    *this /= Norm();
}

inline real Vector2::Norm() const
{
    return sqrt(SquaredNorm());
}

inline real Vector2::SquaredNorm() const
{
    return x*x + y*y;
}

inline Vector2 Vector2::GetNormalized() const
{
    Vector2 temp(*this);
    temp.Normalize();
    return temp;
}

inline real& Vector2::operator[](unsigned i)
{
    RB_ASSERT(i<2);
    return *(&x + i);
}

inline real Vector2::operator[](unsigned i) const
{
    RB_ASSERT(i<2);
    return *(&x + i);
}

inline Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

inline Vector2& Vector2::operator=(const Vector2& vec)
{
    x = vec.x;
    y = vec.y;
    return *this;
}

inline Vector2& Vector2::operator+=(const Vector2& vec)
{
    x += vec.x;
    y += vec.y;
    return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& vec)
{
    x -= vec.x;
    y -= vec.y;
    return *this;
}

inline Vector2& Vector2::operator*=(real scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

inline Vector2& Vector2::operator/=(real scalar)
{
    RB_ASSERT(scalar != 0.0f);
    scalar = 1.0f/scalar;
    return *this *= scalar;
}

inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{
    return (lhs.x==rhs.x) && (lhs.y==rhs.y);
}

inline bool operator!=(const Vector2& lhs, const Vector2& rhs)
{
    return (lhs.x!=rhs.x) || (lhs.y!=rhs.y);
}

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs) += rhs;
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs) -= rhs;
}

inline Vector2 operator*(real scalar, const Vector2& vec)
{
    return Vector2(vec) *= scalar;
}

inline Vector2 operator*(const Vector2& vec, real scalar)
{
    return Vector2(vec) *= scalar;
}

inline Vector2 operator/(const Vector2& vec, real scalar)
{
    return Vector2(vec) /= scalar;
}

inline real Distance(const Vector2& a, const Vector2& b)
{
    return sqrt(SquaredDistance(a, b));
}

inline real SquaredDistance(const Vector2& a, const Vector2& b)
{
    return Sqr(a.x - b.x) + Sqr(a.y - b.y);
}

inline real	DotProduct(const Vector2& vec1,const Vector2& vec2)
{
    return vec1.x*vec2.x + vec1.y*vec2.y;
}

inline real AbsDotProduct(const Vector2& vec1,const Vector2& vec2)
{
    return fabs(DotProduct(vec1, vec2));
}
}
