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

#include "renderbliss/Camera/Film/ImageFilm.h"
#include <cmath>
#include <limits>
#include "renderbliss/Interfaces/IFilter.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/ToneMapping/PhotographicToneMapper.h"
#include "renderbliss/Utils/AtomicOps.h"

namespace renderbliss
{
ImageFilm::ImageFilm(uint32 xResolution, uint32 yResolution, const boost::shared_ptr<IFilter>& filter, const boost::shared_ptr<IToneMapper>& toneMapper)
    : IFilm(xResolution, yResolution, filter), weightedSamples(XResolution(), YResolution()), toneMapper(toneMapper)
{
    if (!this->toneMapper)
    {
        this->toneMapper.reset(new PhotographicToneMapper);
    }
}

void ImageFilm::AddSample(const FilmSample& s)
{
    // Compute sample's raster extent
    real dImageX = s.imageSample[0] - 0.5f; // Discrete coordinate
    real dImageY = s.imageSample[1] - 0.5f; // Discrete coordinate
    int x0 = static_cast<int>( ceil(dImageX - Filter().XWidth()));
    int x1 = static_cast<int>(floor(dImageX + Filter().XWidth()));
    int y0 = static_cast<int>( ceil(dImageY - Filter().YWidth()));
    int y1 = static_cast<int>(floor(dImageY + Filter().YWidth()));
    x0 = std::max(0, x0);
    y0 = std::max(0, y0);
    x1 = std::min(static_cast<int>(XResolution()) - 1, x1);
    y1 = std::min(static_cast<int>(YResolution()) - 1, y1);
    if ((x1 < x0) || (y1 < y0)) { return; }

    // Update weighted samples with sample contribution
    // Atomic operations are used, as the updates may be requested by concurrent threads
    const XYZ& rs = s.radianceSample;
    for (int y = y0; y <= y1; ++y)
    {
        for (int x = x0; x <= x1; ++x)
        {
            real filterWeight = Filter().Weight(static_cast<real>(x)-dImageX, static_cast<real>(y)-dImageY);
            WeightedPixel& wp = weightedSamples(x, y);
            AtomicAdd(&wp.pixel.colour.x, filterWeight*rs.x);
            AtomicAdd(&wp.pixel.colour.y, filterWeight*rs.y);
            AtomicAdd(&wp.pixel.colour.z, filterWeight*rs.z);
            AtomicAdd(&wp.pixel.opacity, filterWeight*s.opacity);
            AtomicAdd(&wp.weightSum, filterWeight);
        }
    }
}

void ImageFilm::AddSamples(const std::vector<FilmSample>& samples)
{
    foreach (const FilmSample& s, samples)
    {
        AddSample(s);
    }
}

void ImageFilm::StorePixels(RGBPixelList& pixels) const
{
    RGBPixel p;
    pixels.clear();
    pixels.reserve(weightedSamples.Width() * weightedSamples.Height());
    for (size_t j = 0; j < weightedSamples.Height(); ++j)
    {
        for (size_t i = 0; i < weightedSamples.Width(); ++i)
        {
            const WeightedPixel& wp = weightedSamples(i, j);
            p.colour = (wp.pixel.colour / wp.weightSum).ToRGB();
            p.opacity = wp.pixel.opacity / wp.weightSum;
            pixels.push_back(p);
        }
    }
    toneMapper->ApplyImagePipeline(pixels);
}
}
