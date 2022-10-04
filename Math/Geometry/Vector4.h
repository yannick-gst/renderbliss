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

#ifndef RENDERBLISS_VECTOR4_H
#define RENDERBLISS_VECTOR4_H

#include <iosfwd>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct Vector3;

struct Vector4
{
    real x, y, z, w;
    static const Vector4 unitX;
    static const Vector4 unitY;
    static const Vector4 unitZ;
    static const Vector4 unitW;
    static const Vector4 zero;

    Vector4();
    Vector4(real val);
    Vector4(const real arr[4]);
    Vector4(const Vector3& vec);
    Vector4(real x, real y, real z, real w);

    void Normalize();
    real Norm() const;
    real SquaredNorm() const;
    Vector4 Normalized() const;

    real        operator[](unsigned i) const;
    real&       operator[](unsigned i);
    Vector4     operator -() const;
    Vector4&    operator =(const Vector4& vec);
    Vector4&    operator+=(const Vector4& vec);
    Vector4&    operator-=(const Vector4& vec);
    Vector4&    operator*=(real scalar);
    Vector4&    operator/=(real scalar);
};

bool    operator==(const Vector4& lhs,const Vector4& rhs);
bool    operator!=(const Vector4& lhs,const Vector4& rhs);
Vector4 operator +(const Vector4& lhs,const Vector4& rhs);
Vector4 operator -(const Vector4& lhs,const Vector4& rhs);
Vector4 operator *(real scalar,const Vector4& vec);
Vector4 operator *(const Vector4& vec,real scalar);
Vector4 operator /(const Vector4& vec,real scalar);

std::ostream& operator<<(std::ostream& os, const Vector4& vec);
}

#include "renderbliss/Math/Geometry/Vector4.inl"

#endif
