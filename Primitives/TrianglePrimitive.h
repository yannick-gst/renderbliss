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

#ifndef RENDERBLISS_TRIANGLE_PRIMITIVE_H
#define RENDERBLISS_TRIANGLE_PRIMITIVE_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "renderbliss/Interfaces/IPrimitive.h"
#include "renderbliss/Math/Sampling/Sampling.h"

namespace renderbliss
{
class  MeshPrimitive;
class  TrianglePrimitive;
struct Vector3;
typedef boost::shared_ptr<const TrianglePrimitive> TrianglePrimitiveConstPtr;
typedef std::vector<TrianglePrimitiveConstPtr> TrianglePrimitiveList;

// A primitive class modelling a triangle in a triangle mesh
class TrianglePrimitive: public IPrimitive
{
public:

    TrianglePrimitive(const MeshPrimitive* mesh, const size_t* vertexIndices);

    real Area() const;

    // Returns the area light attached to this primitive.
    const Luminaire* EmissionProfile() const;

    // Returns the geometric normal (not to be confused with the shading normal).
    Vector3 GeometricNormal() const;

    virtual bool Intersects(const Ray& ray, Intersection& hit) const;
    virtual const IMaterial* Material() const;

    // Randomly samples a point on the triangle
    void SamplePoint(const Sample2D& canonicalRandom, Vector3& sampledPoint, Vector3& outwardShadingNormal) const;

    virtual BoundingBox WorldBound() const;

private:

    const MeshPrimitive* mesh;
    const size_t* vertexIndices;
};
}

#endif
