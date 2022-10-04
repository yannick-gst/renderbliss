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

#ifndef RENDERBLISS_MATHUTILS_H
#define RENDERBLISS_MATHUTILS_H

#include <vector>
#include "renderbliss/Types.h"

namespace renderbliss
{
real Epsilon();
real Infinity();
real Inv3();
real FourInv3();
real Sqrt2();
real Sqrt3();
real InvSqrt2();
real InvSqrt3();
real Inv255();
real Log2();
real InvLog2();
real Pi();
real HalfPi();
real FourthPi();
real TwoPi();
real FourPi();
real InvPi();
real InvTwoPi();
real SqrtPi();
real InvSqrtPi();
real SqrtTwoPi();
real InvSqrtTwoPi();
real RadPerDeg();
real DegPerRad();

// Returns whether a and b are within "delta" of each other
bool AreClose(real a, real b, real delta=Epsilon());

// Returns whether (lhs-rhs) < delta
bool Less(real lhs, real rhs, real delta=Epsilon());

// Returns whether (lhs - rhs) > delta
bool Greater(real lhs, real rhs, real delta=Epsilon());

// Returns the fractional part of a
real Frac(real a);

// Converts an angular measure from degrees to radians
real DegToRad(real degrees);

// Converts an angular measure form radians to degrees
real RadToDeg(real radians);

// Returns x^y
real Pow(real x, real y);

// Returns a (zero-indexed) prime number in the first 1000 prime numbers
uint32 Prime(uint32 i);

// Returns the logarithm in base 2 of a given value
real Log2(real a);

// Evaluates the sine function at  given point
real Sinc(real a);

// Returns a radical inverse in a given base
real RadicalInverse(uint32 n, uint32 base);

// Computes the radical inverse based on a random permutation of digits.
// The caller is responsible  for correctly generating the permutation. The
// permutation is applied to each digit while the inverse is being calculated.
real PermutedRadicalInverse(uint n, uint base, const uint* permutation);

bool IsPowerOfTwo(uint a);
uint NextPowerOfTwo(uint a);
uint RoundToPowerOfTwo(uint a);

template <typename T>
T Sqr(const T& a);

template <typename T>
T Cube(const T& a);

// Returns the sign of a value: -1 for a negative value, 0 for zero, 1 for a positive value
template <typename T>
int Sign(const T& a);

// Clamps a value to a [min, max] range
template <typename T>
void Clamp(const T& min, const T& max, T& val);

// Interpolates between two values by an amount t
template <typename T>
T Lerp(const T& a, const T& b, real t);

// Solves equations of the form: a1*x + a0 = 0
// Returns the number of roots found
uint SolveLinearEquation(real a1, real a0, real& root);

// Solves equations of the form: a2*x^2 + a1*x + a0 = 0
// Returns the number of roots found
uint SolveQuadraticEquation(real a2, real a1, real a0, real roots[2]);

// Solves a 2x2 linear system of matricial form A*X = B (matrices in row-major form)
// Returns true if the system was solved, false otherwise
bool SolveLinearSystem2x2(const real A[2][2], const real B[2], real X[2]);

// Solves a 3x3 linear system of matricial form A*X = B (matrices in row-major form)
// Returns true if the system was solved, false otherwise
bool SolveLinearSystem3x3(const real A[3][3], const real B[3], real X[3]);
}

#include "renderbliss/Math/MathUtils.inl"

#endif
