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

#ifndef RENDERBLISS_RGB_H
#define RENDERBLISS_RGB_H

#include <iosfwd>
#include <boost/array.hpp>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct XYZ;
typedef boost::array<byte,4>  RGBE;
struct RGB
{
    float r, g, b;

    static const RGB black;
    static const RGB white;

    RGB(float val = 0.0f);
    RGB(const XYZ& xyz);
    RGB(const float arr[3]);
    RGB(const RGBE rgbe);
    RGB(float r, float g, float b);

    void Clamp(float low=0.0f, float high=1.0f); // Clamps components to the [low,high] range

    void GammaCorrect(float gamma);
    void InverseGammaCorrect(float gamma);

    bool IsBlack() const;
    bool IsWhite() const;

    float Luminance() const;

    float MaxChannel() const;
    float MinChannel() const;

    XYZ  ToXYZ() const;
    RGBE ToRGBE() const;

    float& operator[](unsigned i);
    float  operator[](unsigned i) const;
    RGB   operator -() const;
    RGB&  operator =(const RGB& colour);
    RGB&  operator+=(const RGB& colour);
    RGB&  operator-=(const RGB& colour);
    RGB&  operator*=(const RGB& colour);
    RGB&  operator*=(float scalar);
    RGB&  operator/=(const RGB& colour);
    RGB&  operator/=(float scalar);
};

bool operator==(const RGB& lhs, const RGB& rhs);
bool operator!=(const RGB& lhs, const RGB& rhs);
RGB  operator +(const RGB& lhs, const RGB& rhs);
RGB  operator -(const RGB& lhs, const RGB& rhs);
RGB  operator *(const RGB& lhs, const RGB& rhs);
RGB  operator *(float scalar, const RGB& colour);
RGB  operator *(const RGB& colour, float scalar);
RGB  operator /(const RGB& lhs, const RGB& rhs);
RGB  operator /(const RGB& colour, float scalar);

std::ostream& operator<<(std::ostream& os, const RGB& colour);
}

#include "renderbliss/Colour/RGB.inl"

#endif
