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

namespace renderbliss
{
inline Basis3::Basis3() : u(Vector3::unitX), v(Vector3::unitY), n(Vector3::unitZ)
{}

inline Basis3::Basis3(const Vector3& u, const Vector3& v, const Vector3& n) : u(u), v(v), n(n)
{}

inline Vector3 Basis3::ToLocal(const Vector3& vec) const
{
    real u = DotProduct(vec, U());
    real v = DotProduct(vec, V());
    real n = DotProduct(vec, N());
    return Vector3(u, v, n);
}

inline Vector3 Basis3::ToLocal(real x, real y, real z) const
{
    return ToLocal(Vector3(x, y, z));
}

inline Vector3 Basis3::ToWorld(const Vector3& vec) const
{
    return vec.x*U() + vec.y*V() + vec.z*N();
}

inline Vector3 Basis3::ToWorld(real u, real v, real n) const
{
    return ToWorld(Vector3(u, v, n));
}

inline const Vector3& Basis3::U() const
{
    return u;
}

inline const Vector3& Basis3::V() const
{
    return v;
}

inline const Vector3& Basis3::N() const
{
    return n;
}
}
