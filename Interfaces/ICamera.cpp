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

#include "renderbliss/Interfaces/ICamera.h"
#include <cmath>
#include "renderbliss/Macros.h"
#include "renderbliss/Interfaces/IFilm.h"
#include "renderbliss/Interfaces/IFilter.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Sampling/SamplingFunctions.h"

namespace renderbliss
{
ICamera::ICamera(const boost::shared_ptr<IFilm>& film, uint32 pixelSamplerWidth)
    : film(film), pixelSamplerWidth(pixelSamplerWidth)
{
    RB_ASSERT(this->film);
}

ICamera::~ICamera()
{
}

IFilm* ICamera::Film() const
{
    RB_ASSERT(film);
    return film.get();
}

void ICamera::GeneratePixelSamples(int x, int y, MersenneTwister& rng, CameraSample& cs) const
{
    GenerateStratifiedSamples(rng, cs.imageSamples, pixelSamplerWidth, pixelSamplerWidth);
    GenerateStratifiedSamples(rng, cs.lensSamples, pixelSamplerWidth, pixelSamplerWidth);
    GenerateStratifiedSamples(rng, cs.timeSamples, SamplesPerPixel());
    // Shift stratified image samples to pixel coordinates
    Shift(cs.imageSamples, static_cast<real>(x), static_cast<real>(y));
    // Decorrelate sample dimensions
    Shuffle(rng, cs.lensSamples);
    Shuffle(rng, cs.timeSamples);
}

void ICamera::GetPixelSampleExtents(int& xStart, int& yStart, int& xEnd, int& yEnd) const
{
    xStart = static_cast<int>(floor(0.5f - film->Filter().XWidth()));
    yStart = static_cast<int>(floor(0.5f - film->Filter().YWidth()));
    xEnd   = static_cast<int>(floor(0.5f + film->XResolution() + film->Filter().XWidth()));
    yEnd   = static_cast<int>(floor(0.5f + film->YResolution() + film->Filter().YWidth()));
}

uint32 ICamera::SamplesPerPixel() const
{
    return pixelSamplerWidth*pixelSamplerWidth;
}
}
