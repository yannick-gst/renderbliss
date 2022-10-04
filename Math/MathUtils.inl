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

#include <cmath>
#include "renderbliss/Macros.h"

namespace renderbliss
{
inline bool AreClose(real a, real b, real delta)
{
	RB_ASSERT(delta >= 0);
    return fabs(a - b) < delta;
}

inline bool Less(real lhs, real rhs, real delta)
{
	RB_ASSERT(delta >= 0);
    return lhs + delta < rhs;
}

inline bool Greater(real lhs, real rhs, real delta)
{
	RB_ASSERT(delta >= 0);
    return lhs - delta > rhs;
}

inline real Frac(real a)
{
    real integerPart;
    return modf(a, &integerPart);
}

inline real DegToRad(real degrees)
{
    return degrees*RadPerDeg();
}

inline real RadToDeg(real radians)
{
    return radians*DegPerRad();
}

inline real Pow(real x, real y)
{
	return (x < 0.0f) ? -pow(-x, y) : pow(x, y);
}

inline real Log2(real a)
{
    return InvLog2()*std::log(a);
}

inline real Sinc(real a)
{
    return (a < 1e-5f) ? 1.0f : sin(a)/a;
}

inline bool IsPowerOfTwo(uint a)
{
    return a && !(a & (a-1));
}

inline uint NextPowerOfTwo(uint a)
{
    return 1 << static_cast<uint>(Log2(static_cast<real>(a)) + 1);
}

inline uint RoundToPowerOfTwo(uint a)
{
    return IsPowerOfTwo(a) ? a : NextPowerOfTwo(a);
}

inline real RadicalInverse(uint32 n, uint32 base)
{
    double inverse = 0.0f;
    double digit = 1.0f/static_cast<double>(base);
    double radical = digit;
    while (n)
    {
        inverse += digit*static_cast<double>(n%base);
        digit *= radical;
        n /= base;
    }
    return static_cast<real>(inverse);
}

inline real PermutedRadicalInverse(uint32 n, uint32 base, const uint32* permutation)
{
    double inverse = 0.0f;
    double digit = 1.0f/static_cast<double>(base);
    double radical = digit;
    while (n)
    {
        inverse += digit*static_cast<double>(permutation[n%base]);
        digit *= radical;
        n /= base;
    }
    return static_cast<real>(inverse);
}

template <typename T>
inline T Sqr(const T& a)
{
    return a*a;
}

template <typename T>
inline T Cube(const T& a)
{
    return a*a*a;
}

template <typename T>
inline int Sign(const T& a)
{
    return (a > 0) ? 1 : (a < 0) ? -1 : 0;
}

template <typename T>
inline void Clamp(const T& a, const T& b, T& val)
{
    val = (val < a) ? a : (val > b) ? b : val;
}

template <typename T>
inline T Lerp(const T& a, const T& b, real t)
{
    return a + (b - a)*t;
}

inline uint SolveLinearEquation(real a1, real a0, real& root)
{
    if (fabs(a1) > 0.0f)
    {
        root = -a0/a1;
		return 1;
    }
    return 0;
}

inline uint SolveQuadraticEquation(real a2, real a1, real a0, real roots[2])
{
    if (a2 == 0)
    {
        return SolveLinearEquation(a1, a0, roots[0]);
    }

    real discriminant = a1*a1 - 4*a2*a0;

    if (discriminant < 0) // There is no real root
    {
        return 0;
    }
    else if (discriminant == 0) // There is one real root
    {
        roots[0] = -0.5f*a1/a2;
        return 1;
    }
    else // There are two real roots
    {
        // Reference:
        // "Numerical Recipes in C"; section 5.6, p. 184.
        // This method is used to avoid numerical errors.
        real q = -0.5f*(a1 + sqrt(discriminant)*Sign(a1));
        roots[0] = q/a2;
        roots[1] = a0/q;
        return 2;
    }
}

inline bool SolveLinearSystem2x2(const real A[2][2], const real B[2], real X[2])
{
    real det = A[1][1]*A[0][0] - A[1][0]*A[0][1];

    if (fabs(det) < Epsilon())
    {
        return false;
    }

    real invDet = 1.0f/det;

    X[0] = (A[1][1]*B[0] - A[0][1]*B[1])*invDet;
    X[1] = (A[0][0]*B[1] - A[1][0]*B[0])*invDet;

    return true;
}
}
