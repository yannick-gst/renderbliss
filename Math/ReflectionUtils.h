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

#ifndef RENDERBLISS_REFLECTION_UTILS_H
#define RENDERBLISS_REFLECTION_UTILS_H

#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"

namespace renderbliss
{
class MersenneTwister;
struct Vector2;
struct Vector3;

// All directions are expressed in a local right-handed coordinate system
// (with the normal corresponding to the z-axis), are of unit length, and
// point away from the surface.

Vector3 HalfwayDirection(const Vector3& incidentDirection, const Vector3& outgoingDirection);

Vector3 DiffuseDirection(MersenneTwister& rng);
Vector3 DiffuseDirection(const Vector2& canonicalRandom);

Vector3 MirrorDirection(const Vector3& d);

Vector3 RefractedDirection(const Vector3& incidentDirection, real iorOrigin, real iorDestination, bool& totalInternalReflection);

real GeometricIorFromComplex(real realPart, real imaginaryPart);

real FresnelConductorTerm(const Vector3& incidentDirection, real ior, real absorptionCoefficient);
real FresnelDielectricTerm(const Vector3& incidentDirection, const Vector3& refractedDirection, real iorOrigin, real iorDestination);

real RefractiveIndexApproximation(real reflectanceAtNormalIncidence);
real AbsorptionCoefficientApproximation(real reflectanceAtNormalIncidence);

real GeometricShadowing(const Vector3& incidentDirection,const Vector3& outgoingDirection);

Spectrum SchlickFresnel(const Vector3& outgoingDirection,const Spectrum& Rs);
}

#endif
