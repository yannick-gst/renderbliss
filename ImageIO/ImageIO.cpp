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

#include "renderbliss/ImageIO/ImageIO.h"
#include <string>
#include <boost/algorithm/string/predicate.hpp>

namespace renderbliss
{
ImageFormat::Enum GetImageFormat(const std::string& fileName)
{
    using namespace boost::algorithm;

    if (iends_with(fileName, ".exr"))
    {
        return ImageFormat::EXR;
    }

    if (iends_with(fileName, ".hdr") || iends_with(fileName, ".pic") || iends_with(fileName, ".rgbea"))
    {
        return ImageFormat::HDR;
    }

    if (iends_with(fileName, ".png"))
    {
        return ImageFormat::PNG;
    }

    if (iends_with(fileName, ".tga"))
    {
        return ImageFormat::TGA;
    }

    return ImageFormat::Invalid;
}

bool LoadImage(const std::string& fileName, RGBPixelList& rgbPixels, uint32& width, uint32& height)
{
    switch (GetImageFormat(fileName))
    {
    case ImageFormat::EXR:
        return LoadEXR(fileName, rgbPixels, width, height);

    case ImageFormat::HDR:
        return LoadHDR(fileName, rgbPixels, width, height);

    case ImageFormat::PNG:
        return LoadPNG(fileName, rgbPixels, width, height);

    case ImageFormat::TGA:
        return LoadTGA(fileName, rgbPixels, width, height);

    default:
        return false;
    }
}

bool LoadImage(const std::string& fileName, ImageFormat::Enum format, RGBPixelList& rgbPixels, uint32& width, uint32& height)
{
    switch (format)
    {
    case ImageFormat::EXR:
        return LoadEXR(fileName, rgbPixels, width, height);

    case ImageFormat::HDR:
        return LoadHDR(fileName, rgbPixels, width, height);

    case ImageFormat::PNG:
        return LoadPNG(fileName, rgbPixels, width, height);

    case ImageFormat::TGA:
        return LoadTGA(fileName, rgbPixels, width, height);

    default:
        return false;
    }
}

bool SaveImage(const std::string& fileName, const RGBPixelList& rgbPixels, uint32 width, uint32 height)
{
    switch (GetImageFormat(fileName))
    {
    case ImageFormat::EXR:
        return SaveEXR(fileName, rgbPixels, width, height);

    case ImageFormat::HDR:
        return SaveHDR(fileName, rgbPixels, width, height);

    case ImageFormat::PNG:
        return SavePNG(fileName, rgbPixels, width, height);

    case ImageFormat::TGA:
        return SaveTGA(fileName, rgbPixels, width, height);

    default:
        return false;
    }
}

bool SaveImage(const std::string& fileName, ImageFormat::Enum format, const RGBPixelList& rgbPixels, uint32 width, uint32 height)
{
    switch (format)
    {
    case ImageFormat::EXR:
        return SaveEXR(fileName, rgbPixels, width, height);

    case ImageFormat::HDR:
        return SaveHDR(fileName, rgbPixels, width, height);

    case ImageFormat::PNG:
        return SavePNG(fileName, rgbPixels, width, height);

    case ImageFormat::TGA:
        return SaveTGA(fileName, rgbPixels, width, height);

    default:
        return false;
    }
}
}
