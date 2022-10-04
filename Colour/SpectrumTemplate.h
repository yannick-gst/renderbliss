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

#ifndef RENDERBLISS_SPECTRUM_TEMPLATE_H
#define RENDERBLISS_SPECTRUM_TEMPLATE_H

#include <iosfwd>
#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <vector>
#include "renderbliss/Colour/ColourMatchingFunctions.h"

namespace renderbliss
{
struct RGB;
struct XYZ;

// Template class for a smooth spectrum
// RGB-to-spectrum reference:
// "An RGB to Spectrum Conversion for Reflectances"; Brian Smits;
// http://www.cs.utah.edu/~bes/papers/color/
template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
class SpectrumTemplate
{
public:

    static const SpectrumTemplate black;
    static const SpectrumTemplate white;
    static size_t SampleCount();
    static float StepSize();

    SpectrumTemplate(float v = 0.0f);
    SpectrumTemplate(const RGB& rgb);
    SpectrumTemplate(const boost::array<float, NumSamples>& samples);
    // Builds a smooth spectrum from an SPD passed as an array of (wavelength, sample) tuples
    SpectrumTemplate(const std::vector<boost::array<float, 2> >& samples);

    void Clamp(float min=0.0f, float max=1.0f);
    bool IsBlack() const;
    bool IsNan() const;
    bool IsWhite() const;

    float Luminance() const;

    RGB ToRGB() const;
    XYZ ToXYZ() const;

    float& operator[](unsigned i);
    float  operator[](unsigned i) const;

    SpectrumTemplate  operator -() const;
    SpectrumTemplate& operator+=(const SpectrumTemplate& spectrum);
    SpectrumTemplate& operator-=(const SpectrumTemplate& spectrum);
    SpectrumTemplate& operator*=(const SpectrumTemplate& spectrum);
    SpectrumTemplate& operator*=(float scalar);
    SpectrumTemplate& operator/=(const SpectrumTemplate& spectrum);
    SpectrumTemplate& operator/=(float scalar);

private:

    BOOST_STATIC_ASSERT(NumSamples > 1);
    BOOST_STATIC_ASSERT(MaxLambda > MinLambda);
    BOOST_STATIC_ASSERT((MinLambda > 0) && (MinLambda >= MinWavelength) && (MinLambda <= MaxWavelength));
    BOOST_STATIC_ASSERT((MaxLambda > 0) && (MaxLambda >= MinWavelength) && (MaxLambda <= MaxWavelength));

    // The RGB to spectrum functions are not automatically computed and must be provided
    static const SpectrumTemplate blueSpectrum;
    static const SpectrumTemplate cyanSpectrum;
    static const SpectrumTemplate greenSpectrum;
    static const SpectrumTemplate magentaSpectrum;
    static const SpectrumTemplate redSpectrum;
    static const SpectrumTemplate yellowSpectrum;
    static const SpectrumTemplate whiteSpectrum;
    static const boost::array<float, NumSamples> X;
    static const boost::array<float, NumSamples> Y;
    static const boost::array<float, NumSamples> Z;
    static const float ySum;

    boost::array<float, NumSamples> samples;
};

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool operator==(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
bool operator!=(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator +(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator -(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator *(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator *(float scalar,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator *(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour,
                                                           float scalar);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator /(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& lhs,
                                                           const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& rhs);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
SpectrumTemplate<MinLambda, MaxLambda, NumSamples> operator /(const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour,
                                                           float scalar);

template <size_t MinLambda, size_t MaxLambda, size_t NumSamples>
std::ostream& operator<<(std::ostream& os, const SpectrumTemplate<MinLambda, MaxLambda, NumSamples>& colour);
}

#include "renderbliss/Colour/SpectrumTemplate.inl"

#endif
