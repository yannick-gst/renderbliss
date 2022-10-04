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

#ifndef RENDERBLISS_ICAMERA_H
#define RENDERBLISS_ICAMERA_H

#include <boost/shared_ptr.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Math/Sampling/Sampling.h"

namespace renderbliss
{
class IFilm;
class MersenneTwister;
struct Ray;

struct CameraSample
{
    SampleList2D imageSamples;
    SampleList2D lensSamples;
    SampleList1D timeSamples;
};

struct PixelSample
{
    Sample2D imageSample;
    Sample2D lensSample;
    Sample1D timeSample;
};

// Base class for cameras
class ICamera
{
public:

    ICamera(const boost::shared_ptr<IFilm>& film, uint32 pixelSamplerWidth);
    virtual ~ICamera();

    IFilm* Film() const;

    // Generates all camera samples necessary to render a pixel at coordinates (x,y)
    void GeneratePixelSamples(int x, int y, MersenneTwister& rng, CameraSample& cs) const;

    // Generates a ray to be traced
    virtual void GenerateRay(const PixelSample& ps, Ray& ray) const = 0;

    // Computes the pixel samples extent: since the reconstruction filter will
    // typically span a number of pixels, the samples extent will be slightly
    // larger than the range of pixels that will actually be output.
    void GetPixelSampleExtents(int& xStart, int& yStart, int& xEnd, int& yEnd) const;

    uint32 SamplesPerPixel() const;

protected:

    boost::shared_ptr<IFilm> film;
    uint32 pixelSamplerWidth;
};
}

#endif
