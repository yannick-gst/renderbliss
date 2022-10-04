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

#ifndef RENDERBLISS_BASIS3_H
#define RENDERBLISS_BASIS3_H

#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
// Orthonormal 3d basis class
class Basis3
{
public:

    Basis3(); // Initializes the basis to (unit X, unit Y, unit Z)

    static Basis3 CreateFromU(const Vector3& u);
    static Basis3 CreateFromV(const Vector3& v);
    static Basis3 CreateFromN(const Vector3& n);
    static Basis3 CreateFromUV(const Vector3& u, const Vector3& v);
    static Basis3 CreateFromVN(const Vector3& v, const Vector3& n);
    static Basis3 CreateFromNU(const Vector3& n, const Vector3& u);

    const Vector3& U() const;
    const Vector3& V() const;
    const Vector3& N() const;

    Vector3 ToLocal(const Vector3& vec) const;
    Vector3 ToWorld(const Vector3& vec) const;
    Vector3 ToLocal(real x, real y, real z) const;
    Vector3 ToWorld(real u, real v, real n) const;

private:

    Vector3 u, v, n;
    Basis3(const Vector3& u, const Vector3& v, const Vector3& n);
};
}

#include "renderbliss/Math/Geometry/Basis3.inl"

#endif
