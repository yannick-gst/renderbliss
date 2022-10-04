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

#ifndef RENDERBLISS_PIXEL_H
#define RENDERBLISS_PIXEL_H

#include <exception>
#include <vector>
#include "renderbliss/Colour/RGB.h"
#include "renderbliss/Colour/XYZ.h"
#include "renderbliss/Colour/Spectrum.h"

namespace renderbliss
{
template <typename ColourBase>
struct Pixel
{
    ColourBase colour;
    float opacity;

    Pixel() : colour(0.0f), opacity(1.0f) {}

    Pixel(const ColourBase& colour) : colour(colour), opacity(1.0f) {}

    Pixel(const ColourBase& colour, real opacity) : colour(colour), opacity(opacity) {}
};

typedef Pixel<RGB> RGBPixel;
typedef Pixel<XYZ> XYZPixel;
typedef Pixel<Spectrum> SpectralPixel;

typedef std::vector<RGBPixel> RGBPixelList;
typedef std::vector<XYZPixel> XYZPixelList;
typedef std::vector<SpectralPixel> SpectralPixelList;
}

#endif
