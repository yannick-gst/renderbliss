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
#include <exception>
#include <IlmImf/ImfArray.h>
#include <IlmImf/ImfRgbaFile.h>
#include <log++/log++.h>

namespace renderbliss
{

bool LoadEXR(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height)
{
    GLOG_INFO << "Loading EXR file " << fileName;

    bool succeeded = false;

    try
    {
        if (!pixels.empty())
        {
            pixels.clear();
        }

        Imf::RgbaInputFile file(fileName.c_str());
        Imath::Box2i dataWindow = file.dataWindow();

        width  = static_cast<uint32>(dataWindow.max.x - dataWindow.min.x + 1);
        height = static_cast<uint32>(dataWindow.max.y - dataWindow.min.y + 1);

        // Read the image
        Imf::Array2D<Imf::Rgba> imfPixels;
        imfPixels.resizeErase(height, width);
        file.setFrameBuffer(&imfPixels[0][0] - dataWindow.min.x - dataWindow.min.y*width, 1, width);
        file.readPixels(dataWindow.min.y, dataWindow.max.y);

        RGBPixel rgbPixel;
        pixels.reserve(static_cast<size_t>(height*width));

        // Store the image pixel values
        for (uint32 j = 0; j < height; ++j)
        {
            for (uint32 i = 0; i < width; ++i)
            {
                rgbPixel.colour.r = float(imfPixels[j][i].r);
                rgbPixel.colour.g = float(imfPixels[j][i].g);
                rgbPixel.colour.b = float(imfPixels[j][i].b);
                rgbPixel.opacity  = float(imfPixels[j][i].a);
                pixels.push_back(rgbPixel);
            }
        }

        succeeded = true;
    }

    catch(std::exception& exc)
    {
        pixels.clear();
        succeeded = false;
        GLOG_ERROR << "Error occurred while loading " << fileName;
        if (strlen(exc.what()))
        {
            GLOG_ERROR << exc.what();
        }
    }

    return succeeded;
}

bool SaveEXR(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height)
{
    GLOG_INFO << "Writing EXR file " << fileName;

    if (pixels.size() < width*height)
    {
        return false;
    }

    bool succeeded = false;

    try
    {
        std::vector<Imf::Rgba> imfPixels;
        imfPixels.reserve(static_cast<size_t>(width*height));

        // Initialize the frame buffer with the image pixel values
        Imf::Rgba rgba;
        uint32 current = 0;
        for (uint32 j = 0; j < height; ++j)
        {
            for (uint32 i = 0; i < width; ++i)
            {
                rgba.r = pixels[current].colour.r;
                rgba.g = pixels[current].colour.g;
                rgba.b = pixels[current].colour.b;
                rgba.a = pixels[current].opacity;
                imfPixels.push_back(rgba);
                ++current;
            }
        }

        // Save the image
        int w  = static_cast<int>(width);
        int h  = static_cast<int>(height);
        Imf::RgbaOutputFile file(fileName.c_str(), w, h, Imf::WRITE_RGBA);
        file.setFrameBuffer(&imfPixels[0], 1, w);
        file.writePixels(h);
        succeeded = true;
    }

    catch(std::exception& exc)
    {
        succeeded = false;
        GLOG_ERROR << "Error occurred while writing " << fileName;
        if (strlen(exc.what()))
        {
            GLOG_ERROR << exc.what();
        }
    }

    return succeeded;
}
}
