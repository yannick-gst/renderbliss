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

#ifndef RENDERBLISS_MESH_PRIMITIVE_H
#define RENDERBLISS_MESH_PRIMITIVE_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Interfaces/IPrimitive.h"

namespace renderbliss
{
class  BoundingBox;
class  IMaterial;
class  TrianglePrimitive;
struct Vector2;
struct Vector3;

typedef boost::shared_ptr<const IMaterial> MaterialConstPtr;
typedef boost::shared_ptr<const IPrimitive> PrimitiveConstPtr;
typedef std::vector<PrimitiveConstPtr> PrimitiveList;
typedef boost::shared_ptr<const TrianglePrimitive> TrianglePrimitiveConstPtr;
typedef std::vector<TrianglePrimitiveConstPtr> TrianglePrimitiveList;

// Mesh primitive class modelling a triangle mesh
class MeshPrimitive: public IPrimitive
{
public:

    static MeshPrimitive CreateFromTriangle(const Vector3& a, const Vector3& b, const Vector3& c, const MaterialConstPtr& material);

    MeshPrimitive(size_t triangleCount,
                 const std::vector<size_t>& vertexIndices,
                 const std::vector<Vector3>& vertices,
                 const std::vector<Vector3>& normals,
                 const std::vector<Vector2>& uvs,
                 const MaterialConstPtr& material);

    // Returns the area light attached to this primitive.
    virtual const Luminaire* EmissionProfile() const;

    // Returns the geometric normal associated to a vertex.
    const Vector3& GeometricNormal(size_t vertexIndex) const;

    // Returns true if shading normals were supplied to construct the mesh.
    // The shading normal for a point in a triangle will be interpolated from
    // the normals associated to each of the triangle's vertices. If no shading
    // normal was provided, the shading normal will be the same as the geometric
    // normal.
    bool HasShadingNormals() const;

    // Returns true if texture coordinates were supplied to construct this mesh.
    // The texture coordinates for a point in the triangle will be interpolated from
    // the uvs associated to each of the triangle's vertices - which will default to
    // (0,0), (0,1) and (1,1) if no texture coordinate was provided.
    bool HasUV() const;

    virtual bool Intersectable() const;

    // Returns the material attached to this primitive.
    virtual const IMaterial* Material() const;

    // Returns the shading normal associated to a vertex, or the geometric normal if
    // no shading normal was supplied to construct the mesh.
    const Vector3& ShadingNormal(size_t vertexIndex) const;

    // Refines the mesh into an array of primitives.
    // Meant to be used to build intersection accelerators.
    void Refine(PrimitiveList& prims) const;

    // Refines the mesh into an array of triangle primitives.
    void Refine(TrianglePrimitiveList& triangles) const;

    // Attaches an area light to the mesh.
    virtual void SetEmissionProfile(const Luminaire* l);

    // Returns the number of triangles in the mesh.
    size_t TriangleCount() const;

    // Returns the texture coordinates associated to a vertex.
    const Vector2& UV(size_t vertexIndex) const;

    // Returns the vertex at an index in the vertex array.
    const Vector3& Vertex(size_t index) const;

    // Returns the number of vertices in the mesh.
    size_t VertexCount() const;

    // Returns the mesh bounding box in world coordinates.
    virtual BoundingBox WorldBound() const;

private:

    size_t triangleCount;
    MaterialConstPtr material;
    std::vector<Vector3> shadingNormals;
    std::vector<Vector3> geometricNormals;
    std::vector<size_t> vertexIndices;
    std::vector<Vector3> vertices;
    std::vector<Vector2> uvs;
    const Luminaire* emissionProfile;
};
}

#endif
