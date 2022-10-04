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

#include "renderbliss/ToneMapping/PhotographicToneMapper.h"
#include <cmath>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"

namespace renderbliss
{
PhotographicToneMapper::PhotographicToneMapper(float keyValue, float dither, float gamma)
    : IToneMapper(dither, gamma), keyValue(fabs(keyValue))
{
}

void PhotographicToneMapper::Map(RGBPixelList& pixels) const
{
    float minLum=0.0f, maxLum=0.0f, Lwa=0.0f;
    ComputeRemarkableLuminances(pixels, minLum, maxLum, Lwa);
    Lwa /= 683.0f;
    float Lwhite2 = Sqr(keyValue*(MaxDisplayableLuminance()/683.0f)/Lwa);
    foreach (RGBPixel& p, pixels)
    {
        float L = keyValue*(p.colour.Luminance()/683.0f)/Lwa;
        float scale = (1.0f + L/Lwhite2) / (1.0f + L);
        p.colour *= scale;
    }
}
}
