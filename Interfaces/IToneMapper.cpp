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

#include "renderbliss/Interfaces/IToneMapper.h"
#include <algorithm>
#include <cmath>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/MersenneTwister.h"

namespace renderbliss
{
IToneMapper::IToneMapper(float dither, float gamma)
    : dither(dither), gamma(gamma)
{
}

IToneMapper::~IToneMapper()
{
}

void IToneMapper::ApplyImagePipeline(RGBPixelList& pixels) const
{
    Map(pixels);
    GammaCorrect(pixels);
    Clamp(pixels);
    Dither(pixels);
}

void IToneMapper::Clamp(RGBPixelList &pixels) const
{
    foreach (RGBPixel& p, pixels) // Handle out of gamut values
    {
        float m = p.colour.MaxChannel();
        if (m > 1.0f) p.colour /= m;
        p.colour.Clamp();
        renderbliss::Clamp(0.0f, 1.0f, p.opacity);
    }
}

void IToneMapper::ComputeRemarkableLuminances(const RGBPixelList& pixels, real& lowest, real& highest, real& logAverage) const
{
    logAverage = 0.0f;
    lowest = Infinity();
    highest = -Infinity();

    foreach (const RGBPixel& p, pixels)
    {
        float lum = p.colour.Luminance();
        if (lum > 0.0f) logAverage += log(lum);
        highest = std::max(highest, lum);
        lowest = std::min(lowest, lum);
    }

    logAverage = exp(logAverage/pixels.size());
}

void IToneMapper::Dither(RGBPixelList& pixels) const
{
    if (dither > 0.0f)
    {
        MersenneTwister rng;
        foreach (RGBPixel& p, pixels)
        {
            float r1 = static_cast<float>(rng.CanonicalRandom()-0.5f);
            float r2 = static_cast<float>(rng.CanonicalRandom()-0.5f);
            float r3 = static_cast<float>(rng.CanonicalRandom()-0.5f);
            p.colour += 2.0f*dither*Inv255()*RGB(r1, r2, r3);
        }
    }
}

void IToneMapper::GammaCorrect(RGBPixelList& pixels) const
{
    foreach (RGBPixel& p, pixels)
    {
        p.colour.GammaCorrect(gamma);
    }
}

float IToneMapper::MaxDisplayableLuminance()
{
    return 100.0f;
}
}
