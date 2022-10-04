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

#ifndef RENDERBLISS_IMAGEFILM_H
#define RENDERBLISS_IMAGEFILM_H

#include "renderbliss/Interfaces/IFilm.h"
#include "renderbliss/Colour/Pixel.h"
#include "renderbliss/Utils/BlockedArray.h"

namespace renderbliss
{
class IToneMapper;

class ImageFilm : public IFilm
{
public:

    ImageFilm(uint32 xResolution, uint32 yResolution, const boost::shared_ptr<IFilter>& filter, const boost::shared_ptr<IToneMapper>& toneMapper);

private:

    struct WeightedPixel
    {
        XYZPixel pixel;
        float weightSum;
        WeightedPixel() : weightSum(0) {}
    };

    BlockedArray<WeightedPixel> weightedSamples;
    boost::shared_ptr<IToneMapper> toneMapper;
    void AddSample(const FilmSample& s);
    void AddSamples(const std::vector<FilmSample>& samples);
    void StorePixels(RGBPixelList& pixels) const;
};
}

#endif
