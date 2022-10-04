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

#ifndef RENDERBLISS_ITONEMAPPER_H
#define RENDERBLISS_ITONEMAPPER_H

#include "renderbliss/Colour/Pixel.h"

namespace renderbliss
{
// Abstract tone mapping class.
// Tone mappers are used to approximate the appearance of high dynamic range images.
class IToneMapper
{
public:

    IToneMapper(float dither=-1.0f, float gamma=2.2f);
    virtual ~IToneMapper();
    void ApplyImagePipeline(RGBPixelList& pixels) const;

protected:

    // Computes the lowest, highest, and log average luminances in a list of pixels
    void ComputeRemarkableLuminances(const RGBPixelList& pixels, float& lowest, float& highest, float& logAverage) const;
    virtual void Map(RGBPixelList& pixels) const = 0;
    static float MaxDisplayableLuminance();

private:

    float dither;
    float gamma;
    void Clamp(RGBPixelList& pixels) const;
    void Dither(RGBPixelList& pixels) const;
    void GammaCorrect(RGBPixelList& pixels) const;
};
}

#endif
