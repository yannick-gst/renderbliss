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

#include <UnitTest++.h>
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Geometry/Ray.h"
#include "renderbliss/Primitives/MeshPrimitive.h"
#include "renderbliss/Primitives/TrianglePrimitive.h"
#include "renderbliss/Materials/LambertianMaterial.h"
#include "renderbliss/Textures/ConstantTexture.h"

namespace
{
using namespace renderbliss;

struct TriangleFixture
{
    TextureConstPtr tex;
    MaterialConstPtr mat;
    MeshPrimitive m;
    TrianglePrimitiveList tlist;
    TriangleFixture() : tex(new ConstantTexture),
                        mat(new LambertianMaterial(tex)),
                        m(MeshPrimitive::CreateFromTriangle(Vector3(250.0f, 250.0f, -1000.0f),
                                                            Vector3(0.0f, 100.0f, -1000.0f),
                                                            Vector3(450.0f, 20.0f, -1000.0f),
                                                            mat))
    {
        m.Refine(tlist);
    }
};

TEST_FIXTURE(TriangleFixture, CheckTriangleArea)
{
    CHECK(tlist.size()==1);
    const TrianglePrimitive* t = tlist.front().get();
    CHECK_CLOSE(43749.9999946525f, t->Area(), Epsilon());
}

TEST_FIXTURE(TriangleFixture, CheckTriangleIntersection)
{
    CHECK(tlist.size()==1);
    const TrianglePrimitive* t = tlist.front().get();
    Ray r(Vector3(250.0f, 100.0f, 300.0f), -Vector3::unitZ);
    Intersection hit;
    CHECK(t->Intersects(r, hit));
    CHECK(hit.triangle==t);
    hit.point = r(r.tmax);
    CHECK_CLOSE(  250.0f, hit.point.x, Epsilon());
    CHECK_CLOSE(  100.0f, hit.point.y, Epsilon());
    CHECK_CLOSE(-1000.0f, hit.point.z, Epsilon());
}

TEST_FIXTURE(TriangleFixture, CheckTriangleNormal)
{
    CHECK(tlist.size()==1);
    const TrianglePrimitive* t = tlist.front().get();
    Vector3 normal = t->GeometricNormal();
    CHECK_CLOSE(0.0f, normal.x, Epsilon());
    CHECK_CLOSE(0.0f, normal.y, Epsilon());
    CHECK_CLOSE(1.0f, normal.z, Epsilon());
}
}
