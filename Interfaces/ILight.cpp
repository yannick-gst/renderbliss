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

#include "renderbliss/Interfaces/ILight.h"
#include "renderbliss/Scene.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Geometry/Vector2.h"

namespace renderbliss
{
OcclusionTester::OcclusionTester()
{
    ray.lookingForShadowHit = true;
}

void OcclusionTester::InitForOcclusionAlongSegment(const Vector3& start, const Vector3& end)
{
    ray.Move(start);
    ray.Redirect(end-start);
    ray.tmin = Epsilon();
    ray.tmax = 1.0f-Epsilon();
}

void OcclusionTester::InitForOcclusionAlongDirection(const Vector3& start, const Vector3& end)
{
    ray.Move(start);
    ray.Redirect(end-start);
    ray.tmin = Epsilon();
    ray.tmax = Infinity();
}

bool OcclusionTester::FindOcclusion(const Scene& s) const
{
    Intersection hit;
    return s.Intersects(ray, hit);
}

LightSamplingRecord::LightSamplingRecord(MersenneTwister& rng, const Intersection& hit, const Sample2D& canonicalRandom)
        : pdf(0.0f), rng(rng), hit(hit), canonicalRandom(canonicalRandom)
{
    RB_ASSERT(canonicalRandom[0] >= 0.0f);
    RB_ASSERT(canonicalRandom[0] <  1.0f);
    RB_ASSERT(canonicalRandom[1] >= 0.0f);
    RB_ASSERT(canonicalRandom[1] <  1.0f);
}

ILight::ILight()
{
}

ILight::~ILight()
{
}
}
