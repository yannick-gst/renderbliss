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

#include "renderbliss/Math/ReflectionUtils.h"
#include <algorithm>
#include <cmath>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Vector2.h"
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
Vector3 HalfwayDirection(const Vector3& incidentDirection, const Vector3& outgoingDirection)
{
    return (incidentDirection + outgoingDirection).GetNormalized();
}

Vector3 DiffuseDirection(MersenneTwister& rng)
{
    Vector2 c(rng.CanonicalRandom(), rng.CanonicalRandom());
    return DiffuseDirection(c);
}

Vector3 DiffuseDirection(const Vector2& canonicalRandom)
{
    RB_ASSERT((canonicalRandom[0] >= 0.0f) &&(canonicalRandom[0] < 1.0f));
    RB_ASSERT((canonicalRandom[1] >= 0.0f) &&(canonicalRandom[1] < 1.0f));
    real phi = TwoPi()*canonicalRandom[0];
    real cosTheta = canonicalRandom[1];
    real sinTheta = sqrt(1.0f - cosTheta*cosTheta);
    return SphericalToCartesian(cos(phi), sin(phi), cosTheta, sinTheta);
}

Vector3 MirrorDirection(const Vector3& d)
{
    return Vector3(-d.x, -d.y, d.z);
}

Vector3 RefractedDirection(const Vector3& incidentDirection, real iorOrigin, real iorDestination, bool& totalInternalReflection)
{
    bool entering = incidentDirection.z > static_cast<real>(0.f); // The incident vector is entering the destination medium

    if (!entering)
    {
        std::swap(iorOrigin, iorDestination);
    }

    real ratio = iorOrigin/iorDestination;
    real critical = 1.0f - ratio*ratio*(1.0f - incidentDirection.z*incidentDirection.z);

    if (critical < 0.0f)
    {
        totalInternalReflection = true;
        return MirrorDirection(incidentDirection);
    }

    totalInternalReflection = false;
    real cost = entering ? -sqrt(critical) : sqrt(critical);
    return Vector3(-ratio*incidentDirection.x, -ratio*incidentDirection.y, cost);
}

real GeometricIorFromComplex(real realPart, real imaginaryPart)
{
    real f1 = Sqr(realPart-1.0f) + imaginaryPart*imaginaryPart;
    real f2 = Sqr(realPart+1.0f) + imaginaryPart*imaginaryPart;
    real rootRatio = sqrt(f1/f2);
    return (1.0f+rootRatio)/(1.0f-rootRatio);
}

// Fresnel equations reference:
// "Global Illumination Compendium" by Philip Dutré; p. 29.
// http://www.cs.kuleuven.ac.be/~phil/GI/

real FresnelDielectricTerm(const Vector3& incidentDirection, const Vector3& refractedDirection, real iorOrigin, real iorDestination)
{
    real cosRatio = fabs(incidentDirection.z/refractedDirection.z);
    real invCosRatio = 1.0f/cosRatio;
    real invN = iorOrigin/iorDestination;
    real fParallel = (cosRatio-invN)/(cosRatio+invN);
    real fPerpendicular = (invCosRatio-invN)/(invCosRatio+invN);
    return 0.5f*(fParallel*fParallel + fPerpendicular*fPerpendicular);
}

real FresnelConductorTerm(const Vector3& incidentDirection, real ior, real absorptionCoefficient)
{
    real cosi = incidentDirection.z;
    real cosi2 = cosi*cosi;
    real etaCosI = 2.0f*ior*cosi;
    real etaPlusK = ior*ior + absorptionCoefficient*absorptionCoefficient;
    real fParallel2 = (etaPlusK*cosi2 - etaCosI + 1.0f)/(etaPlusK*cosi2 + etaCosI + 1.0f);
    real fPerpendicular2 = (etaPlusK - etaCosI + cosi2)/(etaPlusK + etaCosI + cosi2);
    return 0.5f*(fParallel2 + fPerpendicular2);
}

real RefractiveIndexApproximation(real reflectanceAtNormalIncidence)
{
    real rootF0 = sqrt(reflectanceAtNormalIncidence);
    return (1+rootF0)/(1-rootF0);
}

real AbsorptionCoefficientApproximation(real reflectanceAtNormalIncidence)
{
    return 2.0f*sqrt(reflectanceAtNormalIncidence/(1.0f-reflectanceAtNormalIncidence));
}

real GeometricShadowing(const Vector3& incidentDirection, const Vector3& outgoingDirection)
{
    Vector3 halfwayDirection = HalfwayDirection(incidentDirection, outgoingDirection);
    real NdotH = fabs(halfwayDirection.z);
    real NdotI = fabs(incidentDirection.z);
    real NdotO = fabs(outgoingDirection.z);
    real HdotI = AbsDotProduct(halfwayDirection, incidentDirection);
    return (std::min)(static_cast<real>(1.0f), (std::min)(2.0f*NdotH*NdotI/HdotI, 2.0f*NdotH*NdotO/NdotI));
}

Spectrum SchlickFresnel(const Vector3& outgoingDirection, const Spectrum& Rs)
{
    real coso = 1.0f - fabs(outgoingDirection.z);
    return Rs + (Spectrum::white - Rs)*coso*coso*coso*coso*coso;
}
}
