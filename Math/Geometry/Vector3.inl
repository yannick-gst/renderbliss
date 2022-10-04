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
inline Vector3::Vector3()
{}

inline Vector3::Vector3(real val) : x(val), y(val), z(val)
{}

inline Vector3::Vector3(const real arr[3]) :x(arr[0]), y(arr[1]), z(arr[2])
{}

inline Vector3::Vector3(real x, real y, real z) : x(x), y(y), z(z)
{}

inline void Vector3::Normalize()
{
    *this /= Norm();
}

inline real Vector3::Norm() const
{
    return sqrt(SquaredNorm());
}

inline real Vector3::SquaredNorm() const
{
    return x*x + y*y + z*z;
}

inline Vector3 Vector3::GetNormalized() const
{
    Vector3 temp(*this);
    temp.Normalize();
    return temp;
}

inline real& Vector3::operator[](unsigned i)
{
    RB_ASSERT(i<3);
    return *(&x + i);
}

inline real Vector3::operator[](unsigned i) const
{
    RB_ASSERT(i<3);
    return *(&x + i);
}

inline Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

inline Vector3& Vector3::operator=(const Vector3& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

inline Vector3& Vector3::operator+=(const Vector3& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

inline Vector3& Vector3::operator*=(real scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

inline Vector3& Vector3::operator/=(real scalar)
{
    RB_ASSERT(scalar != 0.0f);
    scalar = 1.0f/scalar;
    return *this *= scalar;
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs)
{
    return (lhs.x==rhs.x) && (lhs.y==rhs.y) && (lhs.z==rhs.z);
}

inline bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
    return (lhs.x!=rhs.x) || (lhs.y!=rhs.y) || (lhs.z!=rhs.z);
}

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(lhs) += rhs;
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(lhs) -= rhs;
}

inline Vector3 operator*(real scalar, const Vector3& vec)
{
    return Vector3(vec) *= scalar;
}

inline Vector3 operator*(const Vector3& vec, real scalar)
{
    return Vector3(vec) *= scalar;
}

inline Vector3 operator/(const Vector3& vec, real scalar)
{
    return Vector3(vec) /= scalar;
}

inline real	DotProduct(const Vector3& vec1,const Vector3& vec2)
{
    return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

inline real AbsDotProduct(const Vector3& vec1,const Vector3& vec2)
{
    return fabs(DotProduct(vec1, vec2));
}

inline Vector3 CrossProduct(const Vector3& vec1,const Vector3& vec2)
{
    return Vector3(vec1.y*vec2.z - vec1.z*vec2.y,
                   vec1.z*vec2.x - vec1.x*vec2.z,
                   vec1.x*vec2.y - vec1.y*vec2.x);
}

inline real Distance(const Vector3& a, const Vector3& b)
{
    return sqrt(SquaredDistance(a, b));
}

inline real SquaredDistance(const Vector3& a, const Vector3& b)
{
    return	Sqr(a.x - b.x) + Sqr(a.y - b.y) + Sqr(a.z - b.z);
}

inline Vector3 SphericalToCartesian(real phi, real theta, real r)
{
    real sinTheta = sin(theta);
    return r*Vector3(cos(phi)*sinTheta, sin(phi)*sinTheta, cos(theta));
}

inline Vector3 SphericalToCartesian(real cosPhi, real sinPhi, real cosTheta, real sinTheta, real r)
{
    RB_ASSERT((static_cast<real>(-1.0f) <= cosPhi) && (cosPhi <= static_cast<real>(1.0f)));
    RB_ASSERT((static_cast<real>(-1.0f) <= sinPhi) && (sinPhi <= static_cast<real>(1.0f)));
    RB_ASSERT((static_cast<real>(-1.0f) <= cosTheta) && (cosTheta <= static_cast<real>(1.0f)));
    RB_ASSERT((static_cast<real>(-1.0f) <= sinTheta) && (sinTheta <= static_cast<real>(1.0f)));

    return r*Vector3(cosPhi*sinTheta, sinPhi*sinTheta, cosTheta);
}
}
