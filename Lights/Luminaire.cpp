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

#include "renderbliss/Lights/Luminaire.h"
#include <algorithm>
#include <cmath>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Basis3.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Primitives/TrianglePrimitive.h"

namespace
{
using namespace renderbliss;

void PhongSampleHemisphere(real exponent, real u1, real u2, Vector3& sampledDirection, real& solidAnglePdf)
{
    real phi = TwoPi()*u1;
    real cosTheta = pow(u2, 1.0f/(exponent+1));
    real sinTheta = sqrt(1.0f - cosTheta*cosTheta);
    sampledDirection = SphericalToCartesian(cos(phi), sin(phi), cosTheta, sinTheta);
    solidAnglePdf = InvTwoPi() * (exponent + 1) * pow(cosTheta, exponent);
}
}

namespace renderbliss
{
Luminaire::Luminaire(const TrianglePrimitiveList& triangles, const Spectrum& radiantExitance, real controlExponent)
    : radiantExitance(radiantExitance), triangles(triangles), controlExponent(controlExponent)
{
    RB_ASSERT(!this->triangles.empty());
    controlExponent = std::max(controlExponent, static_cast<real>(1.0f));
    std::vector<real> areaFunction;
    foreach (const TrianglePrimitiveConstPtr& t, this->triangles)
    {
        RB_ASSERT(t.get());
        areaFunction.push_back(t->Area());
        const_cast<TrianglePrimitive*>(t.get())->SetEmissionProfile(this);
    }
    areaDistribution.reset(new StepFunctionSampler(areaFunction));
}

Spectrum Luminaire::EmittedRadiance(const Vector3& lightNormal, const Vector3& toViewer) const
{
    real dot = DotProduct(toViewer, lightNormal);
    if (dot <= 0.0f) { return Spectrum::black; }
    real phongDensity = (controlExponent+1.0f) * pow(dot,controlExponent) * InvTwoPi();
    return radiantExitance*phongDensity/dot;
}

real Luminaire::Pdf(const Vector3& surfacePoint, const Vector3& toLight) const
{
    real pdf = 0.0f;
    const size_t numTriangles = triangles.size();
    for (size_t i = 0; i < numTriangles; ++i)
    {
        // Check if the triangle is intersected at all
        Ray r(surfacePoint, toLight);
        Intersection lightHit;
        if (!triangles[i]->Intersects(r, lightHit)) continue;
        // Accumulate the PDF with respect to solid angle
        RB_ASSERT(i < areaDistribution->StepCount());
        real area = areaDistribution->FunctionValue(i);
        real d2 = SquaredDistance(r(r.tmax), surfacePoint);
        real dot = AbsDotProduct(-toLight, lightHit.uvn.N());
        real solidAnglePdf = (dot > 0.0f) ? d2/(dot*area) : 0.0f;
        pdf += area*solidAnglePdf;
    }
    pdf /= areaDistribution->FunctionIntegral();
    return pdf;
}

Spectrum Luminaire::Power() const
{
    return radiantExitance*areaDistribution->FunctionIntegral();
}

void Luminaire::SampleIncidentDirection(const Intersection& hit, LightSamplingRecord& lrec) const
{
    lrec.pdf = 0.0f;

    if (!hit.triangle) return;

    // First we must pick a shape to sample from, with a probability
    // based on the ratio of its area to the total area of all the shapes
    const size_t numTriangles = triangles.size();
    size_t pick = areaDistribution->SampleIndex(lrec.rng.CanonicalRandom());
    RB_ASSERT((pick >= 0) && (numTriangles > pick));

    if (hit.triangle == triangles[pick].get()) return; // Avoid self-illumination

    Vector3 lightPoint;
    triangles[pick]->SamplePoint(lrec.canonicalRandom, lightPoint, lrec.shadingNormal);

    lrec.toLight = (lightPoint-hit.point).GetNormalized();
    lrec.occlusionTester.InitForOcclusionAlongSegment(hit.point, lightPoint);
    lrec.pdf = Pdf(hit.point, lrec.toLight);

    if (lrec.pdf > 0.0f)
    {
        lrec.emittedRadiance = EmittedRadiance(lrec.shadingNormal, -lrec.toLight);
    }
}

void Luminaire::SampleOutgoingRay(const real canonicalRandom[5], Vector3& origin, Vector3& direction, Vector3& lightNormal, real& pdf) const
{
    pdf = 0.0f;

    // First we must pick a shape to sample from, with a probability
    // based on the ratio of its area to the total area of all the shapes
    const size_t numTriangles = triangles.size();
    size_t pick = areaDistribution->SampleIndex(canonicalRandom[0]);
    RB_ASSERT((pick >= 0) && (numTriangles > pick));
    real lightPickPdf = areaDistribution->FunctionValue(pick) / areaDistribution->FunctionIntegral();

    // Next we sample the ray origin with respect to surface area
    Sample2D s = {canonicalRandom[1], canonicalRandom[2]};
    triangles[pick]->SamplePoint(s, origin, lightNormal);
    real surfaceAreaPdf = 1.0f/areaDistribution->FunctionValue(pick); // The surface area PDF for this particular shape in the luminaire

    // Finally, we sample the ray direction with respect to solid angle and update the PDF
    if (surfaceAreaPdf > 0.0f)
    {
        real solidAnglePdf = 0.0f;
        PhongSampleHemisphere(controlExponent, canonicalRandom[3], canonicalRandom[4], direction, solidAnglePdf);
        if (solidAnglePdf > 0.0f)
        {
            direction = Basis3::CreateFromN(lightNormal).ToWorld(direction);
            pdf = lightPickPdf * surfaceAreaPdf * solidAnglePdf;
        }
    }
}
}
