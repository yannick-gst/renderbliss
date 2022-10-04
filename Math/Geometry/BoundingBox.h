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

#ifndef RENDERBLISS_BOUNDING_BOX_H
#define RENDERBLISS_BOUNDING_BOX_H

#include <iosfwd>
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
struct Ray;

class BoundingBox
{
public:

    // Note that a collapsed bounding box:
    // 1. bounds nothing
    // 2. is not expandable
    // 3. is not intersectable

    BoundingBox(); // Constructs a collapsed bounding box
    BoundingBox(const Vector3& minCorner, const Vector3& maxCorner);

    Vector3 Center() const;

    void Collapse();
    bool IsCollapsed() const;

    void Expand(real delta);
    Vector3 Extents() const;
    void Enclose(const Vector3& point);
    void Enclose(const BoundingBox& box);

    const Vector3& Min() const;
    const Vector3& Max() const;

    // Returns whether the ray intersects the box, as well as the two
    // t-parameterized (with respect to the ray) intersections points
    bool Intersects(const Ray& ray, real& tEntry, real& tExit) const;

private:

    Vector3 minCorner;
    Vector3 maxCorner;
    bool collapsed;
};

std::ostream& operator<<(std::ostream& os, const BoundingBox& box);
}

#include "renderbliss/Math/Geometry/BoundingBox.inl"

#endif
