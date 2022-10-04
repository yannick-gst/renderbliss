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

#ifndef RENDERBLISS_VECTOR2_H
#define RENDERBLISS_VECTOR2_H

#include <iosfwd>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct Vector2
{
    real x, y;
    static const Vector2 unitX;
    static const Vector2 unitY;
    static const Vector2 zero;

    Vector2();
    Vector2(real val);
    Vector2(const real arr[2]);
    Vector2(real x, real y);

    void Normalize();
    real Norm() const;
    real SquaredNorm() const;
    Vector2 GetNormalized() const;

    real        operator[](unsigned i) const;
    real&       operator[](unsigned i);
    Vector2     operator -() const;
    Vector2&    operator =(const Vector2& vec);
    Vector2&    operator+=(const Vector2& vec);
    Vector2&    operator-=(const Vector2& vec);
    Vector2&    operator*=(real scalar);
    Vector2&    operator/=(real scalar);
};

bool    operator==(const Vector2& lhs,const Vector2& rhs);
bool    operator!=(const Vector2& lhs,const Vector2& rhs);
Vector2 operator +(const Vector2& lhs,const Vector2& rhs);
Vector2 operator -(const Vector2& lhs,const Vector2& rhs);
Vector2 operator *(real scalar,const Vector2& vec);
Vector2 operator *(const Vector2& vec,real scalar);
Vector2 operator /(const Vector2& vec,real scalar);

std::ostream& operator<<(std::ostream& os, const Vector2& vec);

real    Distance(const Vector2& a, const Vector2& b);
real    SquaredDistance(const Vector2& a, const Vector2& b);
real    DotProduct(const Vector2& vec1,const Vector2& vec2);
real    AbsDotProduct(const Vector2& vec1,const Vector2& vec2);
}

#include "renderbliss/Math/Geometry/Vector2.inl"

#endif
