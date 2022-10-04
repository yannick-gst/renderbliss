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

namespace renderbliss
{
inline Ray::Ray()
    : tmin(Epsilon()), tmax(Infinity()), depth(0), lookingForShadowHit(false)
{
    invDirection.x = 1.0f/this->direction.x;
    invDirection.y = 1.0f/this->direction.y;
    invDirection.z = 1.0f/this->direction.z;
}

inline Ray::Ray(const Vector3& origin, const Vector3& direction)
    : origin(origin), direction(direction), tmin(Epsilon()), tmax(Infinity()), depth(0), lookingForShadowHit(false)
{
    invDirection.x = 1.0f/this->direction.x;
    invDirection.y = 1.0f/this->direction.y;
    invDirection.z = 1.0f/this->direction.z;
}

inline Vector3 Ray::operator()(real t) const
{
    return origin + t*direction;
}

inline const Vector3& Ray::Direction() const
{
    return direction;
}

inline const Vector3& Ray::InvertedDirection() const
{
    return invDirection;
}

inline void Ray::Move(const Vector3& origin)
{
    this->origin = origin;
}

inline const Vector3& Ray::Origin() const
{
    return origin;
}

inline void Ray::Redirect(const Vector3& direction)
{
    this->direction = direction;
    invDirection.x = 1.0f/direction.x;
    invDirection.y = 1.0f/direction.y;
    invDirection.z = 1.0f/direction.z;
}
}
