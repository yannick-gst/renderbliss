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

#ifndef RENDERBLISS_INTERSECTION_H
#define RENDERBLISS_INTERSECTION_H

#include "renderbliss/Math/Geometry/Basis3.h"
#include "renderbliss/Math/Geometry/Vector2.h"
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
class IMaterial;
class Luminaire;
class TrianglePrimitive;

struct Intersection
{
    Basis3 uvn; // In this basis, the vector N is the _shading_ normal
    Vector3 point;
    Vector2 uv;
    const Luminaire* emitter;
    const IMaterial* material;
    const TrianglePrimitive* triangle;
    Intersection(): emitter(0), material(0), triangle(0) {}
};
}

#endif
