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

#include "renderbliss/Colour/Data/CornellBox.h"
#include <vector>
#include <boost/array.hpp>

namespace
{
    typedef boost::array<float,2> Sample;
}

// Reference:
// http://www.graphics.cornell.edu/online/box/data.html

namespace renderbliss
{
Spectrum CornellGreen()
{
    static SpectralColour g;
    static bool initialized = false;
    if (!initialized)
    {
        Sample samples[76] =
            {{400, 0.092f}, {404, 0.096f}, {408, 0.098f}, {412, 0.097f}, {416, 0.098f}, {420, 0.095f},
             {424, 0.095f}, {428, 0.097f}, {432, 0.095f}, {436, 0.094f}, {440, 0.097f}, {444, 0.098f},
             {448, 0.096f}, {452, 0.101f}, {456, 0.103f}, {460, 0.104f}, {464, 0.107f}, {468, 0.109f},
             {472, 0.112f}, {476, 0.115f}, {480, 0.125f}, {484, 0.140f}, {488, 0.160f}, {492, 0.187f},
             {496, 0.229f}, {500, 0.285f}, {504, 0.343f}, {508, 0.390f}, {512, 0.435f}, {516, 0.464f},
             {520, 0.472f}, {524, 0.476f}, {528, 0.481f}, {532, 0.462f}, {536, 0.447f}, {540, 0.441f},
             {544, 0.426f}, {548, 0.406f}, {552, 0.373f}, {556, 0.347f}, {560, 0.337f}, {564, 0.314f},
             {568, 0.285f}, {572, 0.277f}, {576, 0.266f}, {580, 0.250f}, {584, 0.230f}, {588, 0.207f},
             {592, 0.186f}, {596, 0.171f}, {600, 0.160f}, {604, 0.148f}, {608, 0.141f}, {612, 0.136f},
             {616, 0.130f}, {620, 0.126f}, {624, 0.123f}, {628, 0.121f}, {632, 0.122f}, {636, 0.119f},
             {640, 0.114f}, {644, 0.115f}, {648, 0.117f}, {652, 0.117f}, {656, 0.118f}, {660, 0.120f},
             {664, 0.122f}, {668, 0.128f}, {672, 0.132f}, {676, 0.139f}, {680, 0.144f}, {684, 0.146f},
             {688, 0.150f}, {692, 0.152f}, {696, 0.157f}, {700, 0.159f}};
        g = SpectralColour(std::vector<Sample>(samples, samples + sizeof(samples) / sizeof(samples[0])));
        initialized = true;
    }
#ifdef SPECTRUM_IS_RGB
    return g.ToRGB();
#else
    return g;
#endif
}

Spectrum CornellLightExitance()
{
    static SpectralColour e;
    static bool initialized = false;
    if (!initialized)
    {
        Sample samples[4] = {{400, 0.0f}, {500, 8.0f}, {600, 15.6f}, {700, 18.4f}};
        e = Pi()*SpectralColour(std::vector<Sample>(samples, samples + sizeof(samples) / sizeof(samples[0])));
        initialized = true;
    }
#ifdef SPECTRUM_IS_RGB
    return e.ToRGB();
#else
    return e;
#endif
}

Spectrum CornellLightReflectance()
{
    static const Spectrum s(0.78f);
    return s;
}

Spectrum CornellRed()
{
    static SpectralColour r;
    static bool initialized = false;
    if (!initialized)
    {
        Sample samples[76] =
            {{400, 0.040f}, {404, 0.046f}, {408, 0.048f}, {412, 0.053f}, {416, 0.049f}, {420, 0.050f},
             {424, 0.053f}, {428, 0.055f}, {432, 0.057f}, {436, 0.056f}, {440, 0.059f}, {444, 0.057f},
             {448, 0.061f}, {452, 0.061f}, {456, 0.060f}, {460, 0.062f}, {464, 0.062f}, {468, 0.062f},
             {472, 0.061f}, {476, 0.062f}, {480, 0.060f}, {484, 0.059f}, {488, 0.057f}, {492, 0.058f},
             {496, 0.058f}, {500, 0.058f}, {504, 0.056f}, {508, 0.055f}, {512, 0.056f}, {516, 0.059f},
             {520, 0.057f}, {524, 0.055f}, {528, 0.059f}, {532, 0.059f}, {536, 0.058f}, {540, 0.059f},
             {544, 0.061f}, {548, 0.061f}, {552, 0.063f}, {556, 0.063f}, {560, 0.067f}, {564, 0.068f},
             {568, 0.072f}, {572, 0.080f}, {576, 0.090f}, {580, 0.099f}, {584, 0.124f}, {588, 0.154f},
             {592, 0.192f}, {596, 0.255f}, {600, 0.287f}, {604, 0.349f}, {608, 0.402f}, {612, 0.443f},
             {616, 0.487f}, {620, 0.513f}, {624, 0.558f}, {628, 0.584f}, {632, 0.620f}, {636, 0.606f},
             {640, 0.609f}, {644, 0.651f}, {648, 0.612f}, {652, 0.610f}, {656, 0.650f}, {660, 0.638f},
             {664, 0.627f}, {668, 0.620f}, {672, 0.630f}, {676, 0.628f}, {680, 0.642f}, {684, 0.639f},
             {688, 0.657f}, {692, 0.639f}, {696, 0.635f}, {700, 0.642f}};
        r = SpectralColour(std::vector<Sample>(samples, samples + sizeof(samples) / sizeof(samples[0])));
        initialized = true;
    }
#ifdef SPECTRUM_IS_RGB
    return r.ToRGB();
#else
    return r;
#endif
}

Spectrum CornellWhite()
{
    static SpectralColour w;
    static bool initialized = false;
    if (!initialized)
    {
        Sample samples[76] =
            {{400, 0.343f}, {404, 0.445f}, {408, 0.551f}, {412, 0.624f}, {416, 0.665f}, {420, 0.687f},
             {424, 0.708f}, {428, 0.723f}, {432, 0.715f}, {436, 0.710f}, {440, 0.745f}, {444, 0.758f},
             {448, 0.739f}, {452, 0.767f}, {456, 0.777f}, {460, 0.765f}, {464, 0.751f}, {468, 0.745f},
             {472, 0.748f}, {476, 0.729f}, {480, 0.745f}, {484, 0.757f}, {488, 0.753f}, {492, 0.750f},
             {496, 0.746f}, {500, 0.747f}, {504, 0.735f}, {508, 0.732f}, {512, 0.739f}, {516, 0.734f},
             {520, 0.725f}, {524, 0.721f}, {528, 0.733f}, {532, 0.725f}, {536, 0.732f}, {540, 0.743f},
             {544, 0.744f}, {548, 0.748f}, {552, 0.728f}, {556, 0.716f}, {560, 0.733f}, {564, 0.726f},
             {568, 0.713f}, {572, 0.740f}, {576, 0.754f}, {580, 0.764f}, {584, 0.752f}, {588, 0.736f},
             {592, 0.734f}, {596, 0.741f}, {600, 0.740f}, {604, 0.732f}, {608, 0.745f}, {612, 0.755f},
             {616, 0.751f}, {620, 0.744f}, {624, 0.731f}, {628, 0.733f}, {632, 0.744f}, {636, 0.731f},
             {640, 0.712f}, {644, 0.708f}, {648, 0.729f}, {652, 0.730f}, {656, 0.727f}, {660, 0.707f},
             {664, 0.703f}, {668, 0.729f}, {672, 0.750f}, {676, 0.760f}, {680, 0.751f}, {684, 0.739f},
             {688, 0.724f}, {692, 0.730f}, {696, 0.740f}, {700, 0.737f}};
        w = SpectralColour(std::vector<Sample>(samples, samples + sizeof(samples) / sizeof(samples[0])));
        initialized = true;
    }
#ifdef SPECTRUM_IS_RGB
    return w.ToRGB();
#else
    return w;
#endif
}
}
