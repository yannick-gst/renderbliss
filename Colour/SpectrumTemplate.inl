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

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <boost/math/special_functions/fpclassify.hpp>
#include "renderbliss/Macros.h"
#include "renderbliss/Colour/RGB.h"
#include "renderbliss/Colour/XYZ.h"
#include "renderbliss/Math/MathUtils.h"

namespace renderbliss
{
template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
const SpectrumTemplate<MinLambda, MaxLambda, NumSamples> SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::black(0.0f);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
const SpectrumTemplate<MinLambda, MaxLambda, NumSamples> SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::white(1.0f);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::SpectrumTemplate(float v)
{
    samples.assign(v);
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::SpectrumTemplate(const RGB& rgb)
{
    samples.assign(0.0f);

    if (rgb == RGB::black)
    {
        return;
    }

    // From: "An RGB to Spectrum Conversion for Reflectances"; Brian Smits;
    // http://www.cs.utah.edu/~bes/papers/color/

    const float& red = rgb.r;
    const float& green = rgb.g;
    const float& blue = rgb.b;
    if ((red <= green) && (red <= blue))
    {
        *this += red*whiteSpectrum;
        if (green <= blue)
        {
            *this += (green - red)*cyanSpectrum;
            *this += (blue - green)*blueSpectrum;
        }
        else
        {
            *this += (blue - red)*cyanSpectrum;
            *this += (green - blue)*greenSpectrum;
        }
    }
    else if ((green <= red) && (green <= blue))
    {
        *this += green*whiteSpectrum;
        if (red <= blue)
        {
            *this += (red - green)*magentaSpectrum;
            *this += (blue - red)*blueSpectrum;
        }
        else
        {
            *this += (blue - green)*magentaSpectrum;
            *this += (red - blue)*redSpectrum;
        }
    }
    else // (blue <= red) && (blue <= green)
    {
        *this += blue*whiteSpectrum;
        if (red <= green)
        {
            *this += (red - blue)*yellowSpectrum;
            *this += (green - red)*greenSpectrum;
        }
        else
        {
            *this += (green - blue)*yellowSpectrum;
            *this += (red - green)*redSpectrum;
        }
    }

    Clamp(0.0f, Infinity());
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::SpectrumTemplate(const boost::array<float, NumSamples>& samples) : samples(samples)
{
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::SpectrumTemplate(const std::vector<boost::array<float, 2> >& samples)
{
    if (samples.empty())
    {
        return;
    }

    // Sort the given SPD by wavelength
    std::vector<boost::array<float, 2> > workingSamples = samples;
    struct SamplesComparer : std::binary_function<boost::array<float, 2>, boost::array<float, 2>, bool>
    {
        bool operator()(const boost::array<float, 2>& lhs, const boost::array<float, 2>& rhs) const
        {
            return lhs[0] < rhs[0];
        }
    };
    std::sort(workingSamples.begin(), workingSamples.end(), SamplesComparer());

    for (size_t i = 0; i < NumSamples; ++i)
    {
        // Compute average value of given SPD over the current wavelength range
        float rangeStart = renderbliss::Lerp<float>(static_cast<float>(MinLambda), static_cast<float>(MaxLambda), static_cast<float>( i )/static_cast<float>(NumSamples));
        float rangeEnd   = renderbliss::Lerp<float>(static_cast<float>(MinLambda), static_cast<float>(MaxLambda), static_cast<float>(i+1)/static_cast<float>(NumSamples));
        float rangeAverage = 0.0f;

        if (rangeEnd <= workingSamples.front()[0]) // Use a single sample if we're out of bounds
        {
            rangeAverage = workingSamples.front()[1];
        }
        else if (rangeStart >= workingSamples.back()[0]) // Use a single sample if we're out of bounds
        {
            rangeAverage = workingSamples.back()[1];
        }
        else
        {
            float rangeSum = 0.0f;

            // Add contributions of constant segments before and after the given SPD
            if (rangeStart <= workingSamples.front()[0])
            {
                rangeSum += workingSamples.front()[1]*(workingSamples.front()[0] - rangeStart);
            }
            if (rangeEnd >= workingSamples.back()[0])
            {
                rangeSum += workingSamples.back()[1]*(rangeEnd - workingSamples.back()[0]);
            }

            // Advance to the first relevant wavelength segment
            size_t i = 0;
            while ((i+1 < workingSamples.size()) && (rangeStart > workingSamples[i+1][0]))
            {
                ++i;
            }

            // Add contributions from sample segments
            for (; (i+1 < workingSamples.size()) && (rangeEnd >= workingSamples[i][0]); ++i)
            {
                float segmentStart = std::max(rangeStart, workingSamples[i][0]);
                float segmentEnd = std::min(rangeEnd, workingSamples[i+1][0]);
                float contribution1 = renderbliss::Lerp<float>(workingSamples[i][1], workingSamples[i+1][1], (segmentStart - workingSamples[i][0]) / (workingSamples[i+1][0] - workingSamples[i][0]));
                float contribution2 = renderbliss::Lerp<float>(workingSamples[i][1], workingSamples[i+1][1], (segmentEnd - workingSamples[i][0]) / (workingSamples[i+1][0] - workingSamples[i][0]));
                float segmentAverage = 0.5f*(contribution1 + contribution2);
                rangeSum += segmentAverage*(segmentEnd - segmentStart);
            }

            rangeAverage = rangeSum / (rangeEnd-rangeStart);
        }

        this->samples[i] = rangeAverage;
    }
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
void SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::Clamp(float min, float max)
{
    foreach (float& s, samples)
    {
        renderbliss::Clamp<float>(min, max, s);
    }
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::IsBlack() const
{
    return *this == black;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::IsNan() const
{
    foreach (float amplitude, samples)
    {
        if (boost::math::isnan<float>(amplitude))
        {
            return true;
        }
    }
    return false;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::IsWhite() const
{
    return *this == white;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
float SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::Luminance() const
{
    float y = 0.0f;
    for (size_t i = 0; i < NumSamples; ++i)
    {
        y += Y[i]*samples[i];
    }
    y /= ySum;
    return 683.0f*y;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
RGB SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::ToRGB() const
{
    return ToXYZ().ToRGB();
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
XYZ SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::ToXYZ() const
{
    XYZ xyz(0.0f);
    for (size_t i = 0; i < NumSamples; ++i)
    {
        xyz.x += samples[i]*X[i];
        xyz.y += samples[i]*Y[i];
        xyz.z += samples[i]*Z[i];
    }
    return xyz/ySum;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
float& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator[](unsigned i)
{
    RB_ASSERT(i < NumSamples);
    return samples[i];
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
float SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator[](unsigned i) const
{
    RB_ASSERT(i < NumSamples);
    return samples[i];
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> SpectrumTemplate<MinLambda, MaxLambda, NumSamples>:: operator -() const
{
    return -1*(*this);
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator+=(const SpectrumTemplate& spectrum)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        samples[i] += spectrum.samples[i];
    }
    return *this;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator-=(const SpectrumTemplate& spectrum)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        samples[i] -= spectrum.samples[i];
    }
    return *this;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator*=(const SpectrumTemplate& spectrum)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        samples[i] *= spectrum.samples[i];
    }
    return *this;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator*=(float scalar)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        samples[i] *= scalar;
    }
    return *this;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator/=(const SpectrumTemplate& spectrum)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        RB_ASSERT(spectrum.samples[i] != 0.0f);
        samples[i] /= spectrum.samples[i];
    }
    return *this;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::operator/=(float scalar)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        RB_ASSERT(scalar != 0.0f);
        samples[i] /= scalar;
    }
    return *this;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
size_t SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::SampleCount()
{
    return NumSamples;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
float SpectrumTemplate<MinLambda, MaxLambda, NumSamples>::StepSize()
{
    return static_cast<float>(MaxLambda-MinLambda) / SampleCount();
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool operator==(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs, const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs)
{
    for (size_t i = 0; i < NumSamples; ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool operator!=(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs, const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs)
{
    return !(lhs == rhs);
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator +(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(lhs) += rhs;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator -(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(lhs) -= rhs;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator *(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(lhs) *= rhs;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator *(float scalar,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(colour) *= scalar;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator *(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour,
                                                           float scalar)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(colour) *= scalar;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator /(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(lhs) /= rhs;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator /(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour,
                                                           float scalar)
{
    return SpectrumTemplate<MinLambda, MaxLambda, NumSamples>(colour) /= scalar;
}

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
std::ostream& operator<<(std::ostream& os, const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour)
{
    os << '(';
    for (size_t i = 0; i < NumSamples; ++i)
    {
        os << samples[i];
        if (i+1 < NumSamples)
        {
            os << ', ';
        }
    }
    os << ')';
}
}
