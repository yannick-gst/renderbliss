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

#include "renderbliss/Primitives/MeshPrimitive.h"
#include "renderbliss/Math/Geometry/Vector2.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Math/Geometry/BoundingBox.h"
#include "renderbliss/Primitives/TrianglePrimitive.h"

namespace renderbliss
{
MeshPrimitive::MeshPrimitive(size_t triangleCount, const std::vector<size_t>& vertexIndices,
                           const std::vector<Vector3>& vertices, const std::vector<Vector3>& shadingNormals,
                           const std::vector<Vector2>& uvs, const MaterialConstPtr& material)
    : triangleCount(triangleCount), material(material), shadingNormals(shadingNormals), vertexIndices(vertexIndices), vertices(vertices), uvs(uvs), emissionProfile(0)
{
    RB_ASSERT(material.get());
    const size_t nTriangles = TriangleCount();
    geometricNormals.reserve(nTriangles);
    for (size_t i = 0; i < nTriangles; ++i)
    {
        const Vector3& a = Vertex(vertexIndices[3*i]);
        const Vector3& b = Vertex(vertexIndices[3*i+1]);
        const Vector3& c = Vertex(vertexIndices[3*i+2]);
        Vector3 n = CrossProduct(b-a, c-a).GetNormalized();
        geometricNormals.push_back(n);
    }
}

MeshPrimitive MeshPrimitive::CreateFromTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c, const MaterialConstPtr& material)
{
    std::vector<Vector3> vertices;
    std::vector<size_t> vertexIndices;

    vertices.push_back(a);
    vertexIndices.push_back(0);

    vertices.push_back(b);
    vertexIndices.push_back(1);

    vertices.push_back(c);
    vertexIndices.push_back(2);

    return MeshPrimitive(1, vertexIndices, vertices, std::vector<Vector3>(), std::vector<Vector2>(), material);
}

const Luminaire* MeshPrimitive::EmissionProfile() const
{
    return emissionProfile;
}

const Vector3& MeshPrimitive::GeometricNormal(size_t vertexIndex) const
{
    RB_ASSERT(vertexIndex < vertices.size());
    size_t normalIndex = vertexIndex % TriangleCount();
    RB_ASSERT(normalIndex < geometricNormals.size());
    return geometricNormals[normalIndex];
}

bool MeshPrimitive::HasShadingNormals() const
{
    return !shadingNormals.empty();
}

bool MeshPrimitive::HasUV() const
{
    return !uvs.empty();
}

bool MeshPrimitive::Intersectable() const
{
    return false;
}

const IMaterial* MeshPrimitive::Material() const
{
    return material.get();
}

const Vector3& MeshPrimitive::ShadingNormal(size_t vertexIndex) const
{
    RB_ASSERT(vertexIndex < vertices.size());
    return HasShadingNormals() ? shadingNormals[vertexIndex] : GeometricNormal(vertexIndex);
}

void MeshPrimitive::Refine(PrimitiveList& prims) const
{
    if (vertexIndices.empty() || vertices.empty() || !TriangleCount()) return;
    prims.reserve(TriangleCount());
    prims.clear();
    for (size_t i = 0; i < TriangleCount(); ++i)
    {
        TrianglePrimitiveConstPtr t(new TrianglePrimitive(this, &vertexIndices[3*i]));
        prims.push_back(t);
    }
}

void MeshPrimitive::Refine(TrianglePrimitiveList& triangles) const
{
    if (vertexIndices.empty() || vertices.empty() || !TriangleCount()) return;
    triangles.reserve(TriangleCount());
    triangles.clear();
    for (size_t i = 0; i < TriangleCount(); ++i)
    {
        TrianglePrimitiveConstPtr t(new TrianglePrimitive(this, &vertexIndices[3*i]));
        triangles.push_back(t);
    }
}

void MeshPrimitive::SetEmissionProfile(const Luminaire* l)
{
    RB_ASSERT(l);
    emissionProfile = l;
}

size_t MeshPrimitive::TriangleCount() const
{
    return triangleCount;
}

const Vector2& MeshPrimitive::UV(size_t vertexIndex) const
{
    RB_ASSERT(vertexIndex < vertices.size());
    return uvs[vertexIndex];
}

const Vector3& MeshPrimitive::Vertex(size_t index) const
{
    assert(index < vertices.size());
    return vertices[index];
}

size_t MeshPrimitive::VertexCount() const
{
    return vertices.size();
}

BoundingBox MeshPrimitive::WorldBound() const
{
    BoundingBox box;
    for (std::vector<Vector3>::const_iterator iter = vertices.begin() ; iter != vertices.end(); ++iter)
    {
        box.Enclose(*iter);
    }
    return box;
}
}
