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

#include <algorithm>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"

namespace renderbliss
{
inline RGB::RGB(float val) : r(val), g(val), b(val)
{}

inline RGB::RGB(const float arr[3]) : r(arr[0]), g(arr[1]), b(arr[2])
{}

// Based on code by Bruce Walter
// http://www.graphics.cornell.edu/~bjw/rgbe.html
inline RGB::RGB(const RGBE rgbe)
{
    if (rgbe[3]) // Nonzero pixel
    {
        float f = ldexp(1.0f, rgbe[3]-static_cast<int>(128+8));
        r = f*rgbe[0];
        g = f*rgbe[1];
        b = f*rgbe[2];
    }
    else
    {
        r = g = b = 0.0f;
    }
}

inline RGB::RGB(float r, float g, float b) : r(r), g(g), b(b)
{}

inline void RGB::Clamp(float low, float high)
{
    if (low > high) std::swap(low,  high);
    renderbliss::Clamp(low, high, r);
    renderbliss::Clamp(low, high, g);
    renderbliss::Clamp(low, high, b);
}

inline void RGB::GammaCorrect(float gamma)
{
    if ((gamma != 1.0f) && (gamma != 0.0f))
    {
        float invGamma = 1.0f/gamma;
        r = Pow(r, invGamma);
        g = Pow(g, invGamma);
        b = Pow(b, invGamma);
    }
}

inline void RGB::InverseGammaCorrect(float gamma)
{
    if (gamma != 0.0f)
    {
        GammaCorrect(1.0f/gamma);
    }
}

inline bool RGB::IsBlack() const
{
    return *this == RGB::black;
}

inline bool RGB::IsWhite() const
{
    return *this == RGB::white;
}

inline float RGB::Luminance() const
{
    return 683.0f * (0.212671f*r + 0.715160f*g + 0.072169f*b);
}

inline float RGB::MaxChannel() const
{
    return std::max(r, std::max(g, b));
}

inline float RGB::MinChannel() const
{
    return std::min(r, std::min(g, b));
}

// Based on code by Bruce Walter
// http://www.graphics.cornell.edu/~bjw/rgbe.html
inline RGBE RGB::ToRGBE() const
{
    RGBE rgbe;
    float v = *std::max_element(&r, &r+3);

    if (v < 1e-32)
    {
        rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
    }
    else
    {
        int exponent = 0;
        float mantissa = frexp(v, &exponent);
        v = mantissa * 256.0f/v;
        rgbe[0] = static_cast<byte>(r*v);
        rgbe[1] = static_cast<byte>(g*v);
        rgbe[2] = static_cast<byte>(b*v);
        rgbe[3] = static_cast<byte>(exponent + 128);
    }
    return rgbe;
}

inline float RGB::operator[](unsigned i) const
{
    RB_ASSERT(i < 3);
    return *(&r + i);
}

inline float& RGB::operator[](unsigned i)
{
    RB_ASSERT(i < 3);
    return *(&r + i);
}

inline RGB RGB::operator-() const
{
    return RGB(-r, -g, -b);
}

inline RGB& RGB::operator=(const RGB& colour)
{
    r = colour.r;
    g = colour.g;
    b = colour.b;
    return *this;
}

inline RGB& RGB::operator+=(const RGB& colour)
{
    r += colour.r;
    g += colour.g;
    b += colour.b;
    return *this;
}

inline RGB& RGB::operator-=(const RGB& colour)
{
    r -= colour.r;
    g -= colour.g;
    b -= colour.b;
    return *this;
}

inline RGB& RGB::operator*=(const RGB& colour)
{
    r *= colour.r;
    g *= colour.g;
    b *= colour.b;
    return *this;
}

inline RGB& RGB::operator*=(float scalar)
{
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
}

inline RGB& RGB::operator/=(const RGB& colour)
{
    RB_ASSERT(colour.r != 0.0f);
    RB_ASSERT(colour.g != 0.0f);
    RB_ASSERT(colour.b != 0.0f);
    r /= colour.r;
    g /= colour.g;
    b /= colour.b;
    return *this;
}

inline RGB& RGB::operator/=(float scalar)
{
    RB_ASSERT(scalar != 0.0f);
    scalar = 1.0f/scalar;
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
}

inline bool operator==(const RGB& lhs, const RGB& rhs)
{
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}

inline bool operator!=(const RGB& lhs, const RGB& rhs)
{
    return (lhs.r != rhs.r) || (lhs.g != rhs.g) || (lhs.b != rhs.b);
}

inline RGB operator+(const RGB& lhs, const RGB& rhs)
{
    return RGB(lhs) += rhs;
}

inline RGB operator-(const RGB& lhs, const RGB& rhs)
{
    return RGB(lhs) -= rhs;
}

inline RGB operator*(const RGB& lhs, const RGB& rhs)
{
    return RGB(lhs) *= rhs;
}

inline RGB operator*(float scalar, const RGB& colour)
{
    return RGB(colour) *= scalar;
}

inline RGB operator*(const RGB& colour, float scalar)
{
    return RGB(colour) *= scalar;
}

inline RGB operator/(const RGB& lhs, const RGB& rhs)
{
    return RGB(lhs) /= rhs;
}

inline RGB operator/(const RGB& colour, float scalar)
{
    return RGB(colour) /= scalar;
}
}
