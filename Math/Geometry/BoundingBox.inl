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

#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Ray.h"

namespace renderbliss
{
inline BoundingBox::BoundingBox()
{
    Collapse();
}

inline BoundingBox::BoundingBox(const Vector3& minCorner, const Vector3& maxCorner)
{
    this->minCorner.x = std::min(minCorner.x, maxCorner.x);
    this->minCorner.y = std::min(minCorner.y, maxCorner.y);
    this->minCorner.z = std::min(minCorner.z, maxCorner.z);

    this->maxCorner.x = std::max(minCorner.x, maxCorner.x);
    this->maxCorner.y = std::max(minCorner.y, maxCorner.y);
    this->maxCorner.z = std::max(minCorner.z, maxCorner.z);

    collapsed = false;
}

inline Vector3 BoundingBox::Center() const
{
    return 0.5f*(minCorner + maxCorner);
}

inline void BoundingBox::Collapse()
{
    minCorner = Infinity();
    maxCorner = -Infinity();
    collapsed = true;
}

inline void BoundingBox::Enclose(const Vector3& point)
{
    minCorner.x = std::min(minCorner.x, point.x);
    minCorner.y = std::min(minCorner.y, point.y);
    minCorner.z = std::min(minCorner.z, point.z);

    maxCorner.x = std::max(maxCorner.x, point.x);
    maxCorner.y = std::max(maxCorner.y, point.y);
    maxCorner.z = std::max(maxCorner.z, point.z);

    collapsed = false;
}

inline void BoundingBox::Enclose(const BoundingBox& box)
{
    Enclose(box.Min());
    Enclose(box.Max());
}

inline void BoundingBox::Expand(real delta)
{
    if (!IsCollapsed())
    {
        delta = fabs(delta);
        minCorner -= delta;
        maxCorner += delta;
    }
}

inline Vector3 BoundingBox::Extents() const
{
    return Max() - Min();
}

inline bool BoundingBox::IsCollapsed() const
{
    return collapsed;
}

inline const Vector3& BoundingBox::Min() const
{
    return minCorner;
}

inline const Vector3& BoundingBox::Max() const
{
    return maxCorner;
}

inline bool BoundingBox::Intersects(const Ray& ray, real& tEntry, real& tExit) const
{
    if (IsCollapsed())
    {
        return false;
    }

    // From:
    // "Efficiency Issues for Ray Tracing". Brian Smits. University of Utah
    // http://www.cs.utah.edu/~bes/papers/fastRT/

    tEntry = ray.tmin;
    tExit = ray.tmax;

    for (uint i = 0; i < 3; ++i)
    {
        real invRayDir = ray.InvertedDirection()[i];
        real tmin = (Min()[i] - ray.Origin()[i])*invRayDir;
        real tmax = (Max()[i] - ray.Origin()[i])*invRayDir;

        if (tmin > tmax)
        {
            std::swap(tmin, tmax);
        }

        tEntry = std::max(tEntry, tmin);
        tExit = std::min(tExit, tmax);

        if (tEntry > tExit)
        {
            return false;
        }
    }

    return true;
}
}
