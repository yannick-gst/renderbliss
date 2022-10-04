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

#include "renderbliss/Math/Geometry/Basis3.h"
#include "renderbliss/Math/MathUtils.h"

namespace renderbliss
{
// To set the ortho basis from a single vector, we must create a perpendicular
// vector, and then create the third vector by taking the cross product of the
// two previous ones. The ordering of the operands in the cross product operations
// must be consistent with the ordering of the corresponding vectors in the basis.

Basis3 Basis3::CreateFromU(const Vector3& u)
{
    Vector3 u2 = u.GetNormalized();
    Vector3 v = CrossProduct(u2, Vector3::unitX);
    if (v.SquaredNorm() < Epsilon())
    {
        v = CrossProduct(u2, Vector3::unitY);
    }
    v.Normalize();
    Vector3 n = CrossProduct(u2, v);
    return Basis3(u2, v, n);
}

Basis3 Basis3::CreateFromV(const Vector3& v)
{
    Vector3 v2 = v.GetNormalized();
    Vector3 n = CrossProduct(v2, Vector3::unitY);
    if (n.SquaredNorm() < Epsilon())
    {
        n = CrossProduct(v2, Vector3::unitX);
    }
    n.Normalize();
    Vector3 u = CrossProduct(v2, n);
    return Basis3(u, v2, n);
}

Basis3 Basis3::CreateFromN(const Vector3& n)
{
    Vector3 n2 = n.GetNormalized();
    Vector3 u = CrossProduct(n2, Vector3::unitY);
    if (u.SquaredNorm() < Epsilon())
    {
        u = CrossProduct(n2, Vector3::unitX);
    }
    u.Normalize();
    Vector3 v = CrossProduct(n2, u);
    return Basis3(u, v, n2);
}

// To set the ortho basis from two vectors A and B, we get a normalized copy
// of A. Then we create a vector that's perpendicular to the AB plane. The third
// vector in our base is then obtained simply by taking the cross product of the
// two newly created vectors. Again, the ordering of the operands in the cross
// product ops must econsistent with the ordering of the vectors in the basis.

Basis3 Basis3::CreateFromUV(const Vector3& u, const Vector3& v)
{
    Vector3 u2 = u.GetNormalized();
    Vector3 n = (CrossProduct(u2, v)).GetNormalized();
    return Basis3(u2, CrossProduct(n, u2), n);
}

Basis3 Basis3::CreateFromVN(const Vector3& v, const Vector3& n)
{
    Vector3 v2 = v.GetNormalized();
    Vector3 u = (CrossProduct(v2, n)).GetNormalized();
    return Basis3(u, v2, CrossProduct(u, v2));
}

Basis3 Basis3::CreateFromNU(const Vector3& n, const Vector3& u)
{
    Vector3 n2 = n.GetNormalized();
    Vector3 v = (CrossProduct(n2, u)).GetNormalized();
    return Basis3(CrossProduct(v, n2), v, n2);
}
}
