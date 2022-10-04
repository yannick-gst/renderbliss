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

#include "renderbliss/Primitives/TrianglePrimitive.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Math/Geometry/BoundingBox.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Primitives/MeshPrimitive.h"

namespace renderbliss
{
TrianglePrimitive::TrianglePrimitive(const MeshPrimitive* mesh, const size_t* vertexIndices)
    : mesh(mesh), vertexIndices(vertexIndices)
{
    RB_ASSERT(mesh);
    RB_ASSERT(vertexIndices);
}

real TrianglePrimitive::Area() const
{
    const Vector3& a = mesh->Vertex(vertexIndices[0]);
    const Vector3& b = mesh->Vertex(vertexIndices[1]);
    const Vector3& c = mesh->Vertex(vertexIndices[2]);
    return 0.5f*CrossProduct(b-a, c-a).Norm();
}

const Luminaire* TrianglePrimitive::EmissionProfile() const
{
    return mesh->EmissionProfile();
}

Vector3 TrianglePrimitive::GeometricNormal() const
{
    return mesh->GeometricNormal(vertexIndices[0]);
}

bool TrianglePrimitive::Intersects(const Ray& ray, Intersection& hit) const
{
    // References:
    // "Fast, Minimum Storage Ray/Triangle Intersection" by Tomas Moller & Ben Trumbore
    // "Practical Analysis of Optimized Ray-Triangle Intersection" by Tomas Moller
    // Here we use the non-culling version of the algorithm described in the first reference paper

    const Vector3& a = mesh->Vertex(vertexIndices[0]);
    const Vector3& b = mesh->Vertex(vertexIndices[1]);
    const Vector3& c = mesh->Vertex(vertexIndices[2]);

    Vector3 edge1 = b-a;
    Vector3 edge2 = c-a;

    const Vector3& rayDir = ray.Direction();

    // If the determinant is zero then the ray lies in the plane of the triangle
    Vector3 P = CrossProduct(rayDir, edge2);
    real determinant = DotProduct(edge1, P);
    if (fabs(determinant) < Epsilon())
    {
        return false;
    }

    real invDeterminant = 1.0f/determinant;

    // Calculate distance from A to ray origin
    Vector3 T = ray.Origin() - a;

    // Calculate barycentric coordinate b1
    real b1 = invDeterminant*DotProduct(T, P);
    if ((b1 < 0.0f) || (b1 > 1.0f))
    {
        return false;
    }

    // Calculate barycentric coordinate b2
    Vector3 Q = CrossProduct(T, edge1);
    real b2 = invDeterminant*DotProduct(rayDir, Q);
    if ((b2 < 0.0f) || (b1+b2 > 1.0f))
    {
        return false;
    }

    // The ray intersects the triangle: check if the intersection found is the closest.
    real t = invDeterminant*DotProduct(edge2, Q);
    if ((t < ray.tmin) || (t > ray.tmax))
    {
        return false;
    }

    // If we're not looking for a shadow hit, build the hit record
    if (!ray.lookingForShadowHit)
    {
        ray.tmax = t;
        hit.triangle = this;
        Vector2 uv0 = mesh->HasUV() ? mesh->UV(vertexIndices[0]) : Vector2(0.0f, 1.0f);
        Vector2 uv1 = mesh->HasUV() ? mesh->UV(vertexIndices[1]) : Vector2(1.0f, 0.0f);
        Vector2 uv2 = mesh->HasUV() ? mesh->UV(vertexIndices[2]) : Vector2(1.0f, 1.0f);
        real b0 = 1.0f-b1-b2; // Barycentric coordinate b0
        hit.uv = b0*uv0 + b1*uv1 + b2*uv2;
        if (mesh->HasShadingNormals())
        {
            hit.uvn = Basis3::CreateFromN(  b0*mesh->ShadingNormal(vertexIndices[0])
                                          + b1*mesh->ShadingNormal(vertexIndices[1])
                                          + b2*mesh->ShadingNormal(vertexIndices[2]));
        }
        else
        {
            hit.uvn = Basis3::CreateFromN(GeometricNormal());
        }
    }

    return true;
}

const IMaterial* TrianglePrimitive::Material() const
{
    return mesh->Material();
}

void TrianglePrimitive::SamplePoint(const Sample2D& canonicalRandom, Vector3& sampledPoint, Vector3& outwardShadingNormal) const
{
    RB_ASSERT(canonicalRandom[0] >= 0.0f);
    RB_ASSERT(canonicalRandom[0] <  1.0f);
    RB_ASSERT(canonicalRandom[1] >= 0.0f);
    RB_ASSERT(canonicalRandom[1] <  1.0f);

    const Vector3& a = mesh->Vertex(vertexIndices[0]);
    const Vector3& b = mesh->Vertex(vertexIndices[1]);
    const Vector3& c = mesh->Vertex(vertexIndices[2]);

    real tmp = sqrt(1.0f - canonicalRandom[0]);
    real b1 = 1.0f - tmp;
    real b2 = tmp*canonicalRandom[1];
    real b0 = 1.0f - b1 - b2;
    sampledPoint = b0*a + b1*b + b2*c;
    outwardShadingNormal = mesh->HasShadingNormals() ? b0*mesh->ShadingNormal(vertexIndices[0]) + b1*mesh->ShadingNormal(vertexIndices[1]) + b2*mesh->ShadingNormal(vertexIndices[2])
                                                     : GeometricNormal();
}

BoundingBox TrianglePrimitive::WorldBound() const
{
    BoundingBox box;
    box.Enclose(mesh->Vertex(vertexIndices[0]));
    box.Enclose(mesh->Vertex(vertexIndices[1]));
    box.Enclose(mesh->Vertex(vertexIndices[2]));
    box.Expand(Epsilon());
    return box;
}
}
