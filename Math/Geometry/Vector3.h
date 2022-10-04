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

#ifndef RENDERBLISS_VECTOR3_H
#define RENDERBLISS_VECTOR3_H

#include <iosfwd>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct Vector3
{
    real x, y, z;
    static const Vector3 unitX;
    static const Vector3 unitY;
    static const Vector3 unitZ;
    static const Vector3 zero;

    Vector3();
    Vector3(real val);
    Vector3(const real arr[3]);
    Vector3(real x, real y, real z);

    void Normalize();
    real Norm() const;
    real SquaredNorm() const;
    Vector3 GetNormalized() const;

    real        operator[](unsigned i) const;
    real&       operator[](unsigned i);
    Vector3     operator -() const;
    Vector3&    operator =(const Vector3& vec);
    Vector3&    operator+=(const Vector3& vec);
    Vector3&    operator-=(const Vector3& vec);
    Vector3&    operator*=(real scalar);
    Vector3&    operator/=(real scalar);
};

bool    operator==(const Vector3& lhs,const Vector3& rhs);
bool    operator!=(const Vector3& lhs,const Vector3& rhs);
Vector3 operator +(const Vector3& lhs,const Vector3& rhs);
Vector3 operator -(const Vector3& lhs,const Vector3& rhs);
Vector3 operator *(real scalar,const Vector3& vec);
Vector3 operator *(const Vector3& vec,real scalar);
Vector3 operator /(const Vector3& vec,real scalar);

std::ostream& operator<<(std::ostream& os, const Vector3& vec);

real    Distance(const Vector3& a, const Vector3& b);
real    SquaredDistance(const Vector3& a, const Vector3& b);
real    DotProduct(const Vector3& vec1,const Vector3& vec2);
real    AbsDotProduct(const Vector3& vec1,const Vector3& vec2);
Vector3 CrossProduct(const Vector3& vec1,const Vector3& vec2);
Vector3 SphericalToCartesian(real phi, real theta, real r=1.0f);
Vector3 SphericalToCartesian(real cosPhi, real sinPhi, real cosTheta, real sinTheta, real r=1.0f);
}

#include "renderbliss/Math/Geometry/Vector3.inl"

#endif
