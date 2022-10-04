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

#ifndef RENDERBLISS_IMAGEIO_H
#define RENDERBLISS_IMAGEIO_H

#include "renderbliss/Colour/Pixel.h"
#include <string>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct ImageFormat : NonConstructible
{
    enum Enum
    {
        EXR,
        HDR,
        PNG,
        TGA,
        Invalid
    };
};

ImageFormat::Enum GetImageFormat(const std::string& fileName);

// Each of the below functions returns true for success, and false for failure

bool LoadEXR(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height);
bool LoadHDR(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height);
bool LoadPNG(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height);
bool LoadTGA(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height); // Only true-color images are supported
bool LoadImage(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height); // Will deduce the image format from the file extension
bool LoadImage(const std::string& fileName, ImageFormat::Enum format, RGBPixelList& pixels, uint32& width, uint32& height);

bool SaveEXR(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height);
bool SaveHDR(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height);
bool SavePNG(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height);
bool SaveTGA(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height); // Only true-color images are supported
bool SaveImage(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height); // Will deduce the image format from the file extension
bool SaveImage(const std::string& fileName, ImageFormat::Enum format, const RGBPixelList& pixels, uint32 width, uint32 height);
}

#endif
