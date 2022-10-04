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

#include "renderbliss/Colour/Spectrum.h"

#include <numeric>
#include <boost/function.hpp>
#include "renderbliss/Math/MathUtils.h"

namespace
{
using namespace renderbliss;

boost::array<float, SpectralSampleCount> MatchingFunctionRangeAverage(boost::function<float (float)> MatchingFunction)
{
    boost::array<float, SpectralSampleCount> result;
    result.assign(0.0f);
    for (size_t i = 0; i < SpectralSampleCount; ++i)
    {
        float rangeSum = 0.0f;

        float rangeStart = Lerp<float>(static_cast<float>(MinVisibleWavelength),
                                       static_cast<float>(MaxVisibleWavelength),
                                       static_cast<float>( i )/static_cast<float>(SpectralSampleCount));

        float rangeEnd   = Lerp<float>(static_cast<float>(MinVisibleWavelength),
                                       static_cast<float>(MaxVisibleWavelength),
                                       static_cast<float>(i+1)/static_cast<float>(SpectralSampleCount));

        float segmentStart = rangeStart;

        while (segmentStart < rangeEnd)
        {
            float startValue = MatchingFunction(segmentStart);
            float segmentEnd = segmentStart + 1.0f;
            float endValue = MatchingFunction(segmentEnd);
            float segmentAverage = 0.5f*(startValue + endValue);
            rangeSum += segmentAverage;
            segmentStart += 1.f;
        }
        result[i] = rangeSum / (rangeEnd-rangeStart);
    }
    return result;
}

// Data obtained from:
// "An RGB to Spectrum Conversion for Reflectances"; Brian Smits;
// http://www.cs.utah.edu/~bes/papers/color/
const boost::array<float, SpectralSampleCount> Blue()
{
    static const boost::array<float, SpectralSampleCount> b = {1.0f, 1.0f, 0.8916f, 0.3323f, 0.0f, 0.0f, 0.0003f, 0.0369f, 0.0483f, 0.0496f};
    return b;
}

const boost::array<float, SpectralSampleCount> Cyan()
{
    static const boost::array<float, SpectralSampleCount> c = {0.9710f, 0.9426f, 1.0007f, 1.0007f, 1.0007f, 1.0007f, 0.1564f, 0.0f, 0.0f, 0.0f};
    return c;
}

const boost::array<float, SpectralSampleCount> Green()
{
    static const boost::array<float, SpectralSampleCount> g = {0.0f, 0.0f, 0.0273f, 0.7937f, 1.0f, 0.9418f, 0.1719f, 0.0f, 0.0f, 0.0025f};
    return g;
}

const boost::array<float, SpectralSampleCount> Magenta()
{
    static const boost::array<float, SpectralSampleCount> m = {1.0f, 1.0f, 0.9685f, 0.2229f, 0.0f, 0.0458f, 0.8369f, 1.0f, 1.0f, 0.9959f};
    return m;
}

const boost::array<float, SpectralSampleCount> Red()
{
    static const boost::array<float, SpectralSampleCount> r = {0.1012f, 0.0515f, 0.0f, 0.0f, 0.0f, 0.0f, 0.8325f, 1.0149f, 1.0149f, 1.0149f};
    return r;
}

const boost::array<float, SpectralSampleCount> Yellow()
{
    static const boost::array<float, SpectralSampleCount> y = {0.0001f, 0.0f, 0.1088f, 0.6651f, 1.0f, 1.0f, 0.9996f, 0.9586f, 0.9685f, 0.9840f};
    return y;
}

const boost::array<float, SpectralSampleCount> White()
{
    static const boost::array<float, SpectralSampleCount> w = {1.0f, 1.0f, 0.9999f, 0.9993f, 0.9992f, 0.9998f, 1.0f, 1.0f, 1.0f, 1.0f};
    return w;
}
}

namespace renderbliss
{
const SpectralColour SpectralColour::blueSpectrum(Blue());
const SpectralColour SpectralColour::cyanSpectrum(Cyan());
const SpectralColour SpectralColour::greenSpectrum(Green());
const SpectralColour SpectralColour::magentaSpectrum(Magenta());
const SpectralColour SpectralColour::redSpectrum(Red());
const SpectralColour SpectralColour::yellowSpectrum(Yellow());
const SpectralColour SpectralColour::whiteSpectrum(White());
const boost::array<float, SpectralSampleCount> SpectralColour::X = MatchingFunctionRangeAverage(&renderbliss::X);
const boost::array<float, SpectralSampleCount> SpectralColour::Y = MatchingFunctionRangeAverage(&renderbliss::Y);
const boost::array<float, SpectralSampleCount> SpectralColour::Z = MatchingFunctionRangeAverage(&renderbliss::Z);
const float SpectralColour::ySum = std::accumulate(SpectralColour::Y.begin(), SpectralColour::Y.end(), 0.0f);
}
