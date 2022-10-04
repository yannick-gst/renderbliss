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

#ifndef RENDERBLISS_IFILM_H
#define RENDERBLISS_IFILM_H

#include <boost/shared_ptr.hpp>
#include "renderbliss/Colour/Pixel.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Math/Sampling/Sampling.h"

namespace renderbliss
{
class IFilter;

struct FilmSample
{
    XYZ radianceSample;
    Sample2D imageSample;
    float opacity;
};

// Base class to model a film in a camera
class IFilm
{
public:

    // Constructs a film given its resolution in pixels, and a filter for image reconstruction.
    // The filter defaults to a box filter if an invalid filter is passed in the argument list.
    IFilm(uint32 xResolution, uint32 yResolution, const boost::shared_ptr<IFilter>& filter);

    virtual ~IFilm();
    virtual void AddSample(const FilmSample& s) = 0;
    virtual void AddSamples(const std::vector<FilmSample>& samples) = 0;

    float Dx() const; // Returns the inverse of the X resolution
    float Dy() const; // Returns the inverse of the Y resolution

    const IFilter& Filter() const;

    virtual void StorePixels(RGBPixelList& pixels) const = 0;

    uint32 XResolution() const;
    uint32 YResolution() const;

private:

    boost::shared_ptr<IFilter> filter;
    uint32 xRes, yRes;
    float dx, dy;
};
}

#endif
