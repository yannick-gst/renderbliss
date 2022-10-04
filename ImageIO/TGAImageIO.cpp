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
#include <algorithm>
#include <fstream>
#include <strstream>
#include <vector>
#include <log++/log++.h>
#include "renderbliss/Types.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Utils/Utils.h"

// File format reference (available on http://www.wotsit.org):
// "Truevision TGA File Format Specification Version 2.0"
// Technical Manual Version 2.2 January, 1991; Truevision, Inc.

namespace
{
using namespace renderbliss;

bool ReadUncompressedImageData(uint32 imageWidth, uint32 imageHeight, uint32 pixelDepth, std::istream& inputStream, RGBPixelList& pixels)
{
    uint32 nPixelsInImage = imageWidth*imageHeight;
    pixels.reserve(static_cast<size_t>(nPixelsInImage));

    RGBPixel rgbPixel(RGB::black);
    byte bgr[3] = {0};
    byte opacity = 255;

    for (uint32 i = 0; i < nPixelsInImage; ++i)
    {
        opacity = 255;

        if (inputStream.read((char*)bgr, sizeof(bgr)).fail())
        {
            GLOG_ERROR << "Failed to read color triplet.";
            return false;
        }

        if (pixelDepth == 32)
        {
            if (inputStream.read((char*)(&opacity), sizeof(opacity)).fail())
            {
                GLOG_ERROR << "Failed to read opacity channel.";
                return false;
            }
        }

        rgbPixel.colour.b = bgr[0]*Inv255();
        rgbPixel.colour.g = bgr[1]*Inv255();
        rgbPixel.colour.r = bgr[2]*Inv255();
        rgbPixel.opacity  = opacity*Inv255();

        pixels.push_back(rgbPixel);
    }

    return true;
}


bool ReadCompressedImageData(uint32 imageWidth, uint32 imageHeight, uint32 pixelDepth, std::istream& inputStream, RGBPixelList& pixels)
{
    uint32 nPixelsInImage = imageWidth*imageHeight;
    pixels.reserve(static_cast<size_t>(nPixelsInImage));

    RGBPixel rgbPixel(RGB::black);
    byte bgr[3] = {0};
    byte opacity = 255;
    uint32 iPixel = 0;

    while (iPixel < nPixelsInImage)
    {
        byte packetHeader = 0;
        if (inputStream.read((char*)(&packetHeader), 1).fail())
        {
            GLOG_ERROR << "Failed to read packet header.";
            return false;
        }

        if (packetHeader < 128) // It's a RAW packet
        {
            ++packetHeader;
            for (uint32 i = 0; i < packetHeader; ++i, ++iPixel)
            {
                opacity = 255;

                if (inputStream.read((char*)bgr, sizeof(bgr)).fail())
                {
                    GLOG_ERROR << "Failed to read color triplet.";
                    return false;
                }

                if (pixelDepth == 32)
                {
                    if (inputStream.read((char*)(&opacity), sizeof(opacity)).fail())
                    {
                        GLOG_ERROR << "Failed to read opacity channel.";
                        return false;
                    }

                    rgbPixel.opacity = opacity*Inv255();
                }

                rgbPixel.colour.b = bgr[0]*Inv255();
                rgbPixel.colour.g = bgr[1]*Inv255();
                rgbPixel.colour.r = bgr[2]*Inv255();

                pixels.push_back(rgbPixel);
            }
        }
        else // It's an RLE packet
        {
            packetHeader -= 127;

            if (inputStream.read((char*)bgr, sizeof(bgr)).fail())
            {
                GLOG_ERROR << "Failed to read color triplet.";
                return false;
            }

            if (pixelDepth == 32)
            {
                if (inputStream.read((char*)(&opacity), sizeof(opacity)).fail())
                {
                    GLOG_ERROR << "Failed to read opacity channel.";
                    return false;
                }

                rgbPixel.opacity = opacity*Inv255();
            }

            rgbPixel.colour.b = bgr[0]*Inv255();
            rgbPixel.colour.g = bgr[1]*Inv255();
            rgbPixel.colour.r = bgr[2]*Inv255();

            for (uint32 i = 0; i < packetHeader; ++i, ++iPixel)
            {
                pixels.push_back(rgbPixel);
            }
        }
    }

    return true;
}
}


namespace renderbliss
{
bool LoadTGA(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height)
{
    GLOG_INFO << "Loading TGA file " << fileName;

    pixels.clear();

    std::vector<byte> fileData;
    int nBytesInFile = ReadFileIntoMemory(fileName, fileData);
    if (nBytesInFile == -1)
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to load file into memory.";
        return false;
    }

    std::istrstream inputStream((char*)(&fileData[0]), nBytesInFile);

    // Retrieve TGA header
    byte imageHeader[18] = {0};
    if (inputStream.read((char*)imageHeader, sizeof(imageHeader)).fail())
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to read TGA header.";
        return false;
    }

    // Determine if the image type is supported
    byte imageType = imageHeader[2]; // Types supported: uncompressed, true-color (2); and RLE-compressed, true-color (10)
    byte pixelDepth = imageHeader[16]; // Pixel depths suported: 24 and 32 bpp
    if (((imageType != 2) && (imageType != 10)) || ((pixelDepth != 24) && (pixelDepth != 32)))
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": image type not supported.";
        return false;
    }

    // Skip image ID
    byte idLength = imageHeader[0];
    inputStream.seekg(idLength, std::ios_base::cur);

    // Skip color map
    byte colorMapType = imageHeader[1];
    if (colorMapType == 1) // A color map is included with the image
    {
        // Skip first entry index
        inputStream.seekg(2, std::ios_base::cur);

        uint32 nEntriesInColorMap = 0;
        if (inputStream.read((char*)(&nEntriesInColorMap), 2).fail())
        {
            GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to read color map length.";
            return false;
        }

        byte nBitsPerColorMapEntry = 0;
        if (inputStream.read((char*)(&nBitsPerColorMapEntry), sizeof(nBitsPerColorMapEntry)).fail())
        {
            GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to read color map entry size.";
            return false;
        }

        uint32 nBytesColorMap = nEntriesInColorMap * ((nBitsPerColorMapEntry + 7) / 8);
        inputStream.seekg(nBytesColorMap, std::ios_base::cur);
    }

    width = (imageHeader[13] << 8) + imageHeader[12];
    height = (imageHeader[15] << 8) + imageHeader[14];

    if (imageType == 2)
    {
        if (!ReadUncompressedImageData(width, height, pixelDepth, inputStream, pixels))
        {
            GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to read image data.";
            return false;
        }
    }
    else if (!ReadCompressedImageData(width, height, pixelDepth, inputStream, pixels))
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to read image data.";
        return false;
    }

    // Flip loaded image data if necessary
    byte imageDescriptor = imageHeader[17];

    bool isFlippedHorizontally = (imageDescriptor & (1 << 4)) == 1;
    if (isFlippedHorizontally && !FlipArrayHorizontally<RGBPixel>(width, height, pixels))
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to flip image data horizontally.";
        return false;
    }

    bool isFlippedVertically = (imageDescriptor & (1 << 5)) == 0;
    if (isFlippedVertically && !FlipArrayVertically<RGBPixel>(width, height, pixels))
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to flip image data vertically.";
        return false;
    }

    return true;
}


bool SaveTGA(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height)
{
    GLOG_INFO << "Writing TGA file " << fileName;

    if (pixels.size() < width*height)
    {
        return false;
    }

    // Set up the image header
    byte imageHeader[18] = {0};
    imageHeader[2] = 2;     // Uncompressed true-color image
    imageHeader[17] = 0x20; // First pixel in upper left corner
    imageHeader[16] = 32;   // 32 bits per pixel
    imageHeader[15] = static_cast<byte>(height >> 8);
    imageHeader[14] = static_cast<byte>(height - (imageHeader[15] << 8));
    imageHeader[13] = static_cast<byte>(width >> 8);
    imageHeader[12] = static_cast<byte>(width - (imageHeader[13] << 8));

    // Write the image header
    std::ofstream os(fileName.c_str(), std::ios_base::binary);
    if (os.write((char*)imageHeader, sizeof(imageHeader)).fail())
    {
        GLOG_ERROR << "Error saving " << fileName << ": failed to write image header.";
        return false;
    }

    // Write the image data
    foreach (const RGBPixel& currentPixel, pixels)
    {
        byte bgra[4] = {0};
        bgra[0] = static_cast<byte>(currentPixel.colour.b*255.0f);
        bgra[1] = static_cast<byte>(currentPixel.colour.g*255.0f);
        bgra[2] = static_cast<byte>(currentPixel.colour.r*255.0f);
        bgra[3] = static_cast<byte>(currentPixel.opacity*255.0f);
        if (os.write((char*)bgra, sizeof(bgra)).fail())
        {
            GLOG_ERROR << "Error saving " << fileName << ": failed to write image pixel.";
            return false;
        }
    }

    return true;
}
}
