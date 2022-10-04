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

#ifndef RENDERBLISS_PHOTOGRAPHIC_TONE_MAPPER_H
#define RENDERBLISS_PHOTOGRAPHIC_TONE_MAPPER_H

#include "renderbliss/Interfaces/IToneMapper.h"

namespace renderbliss
{
// Maps potentially HDR images to the low dynamic range of photographic print.
// Reference: "Photographic Tone Reproduction for Digital Images" by Erik Reinhard et al.
class PhotographicToneMapper : public IToneMapper
{
public:

    PhotographicToneMapper(float keyValue=0.18f, float dither=-1.0f, float gamma=2.2f);

private:

    float keyValue;
    void Map(RGBPixelList& pixels) const;
};
}

#endif
