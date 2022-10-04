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
#include <fstream>
#include <sstream>
#include <strstream>
#include <log++/log++.h>
#include "renderbliss/Utils/Utils.h"

namespace
{
using namespace renderbliss;

// Flags indicating which fields in an RGBEHeaderInfo are valid
const int rgbeValidProgramType = 0x01;
const int rgbeValidComment     = 0x02;
const int rgbeValidGamma       = 0x04;
const int rgbeValidExposure    = 0x08;

enum { HdrMaxLineLength = 256 };

// RGBEHeaderInfo structure. Stores header information for an HDR image file.
struct RGBEHeaderInfo
{
    int valid;                      // Indicates which fields are valid
    char programType[16];           // Listed at beginning of file to identify it after "#?" (defaults to "RADIANCE")
    char comment[HdrMaxLineLength]; // Comment beginning with "#"
    float gamma;                    // Image has already been gamma corrected with a given gamma (defaults to 1.0 for no correction)
    float exposure;                 // A value of 1.0 in an image corresponds to <exposure> watts/steradian/m^2 (defaults to 1.0f)
};

// Reads RGBE pixels from a stream
bool ReadFlatRgbePixels(std::istream& is, RGBPixelList& pixels, uint32 scanlineWidth, uint32 nScanlines, bool flippedX, bool flippedY)
{
    RGBE rgbe = {0};
    RGBPixel rgbPixel(RGB::black);

    for (uint32 j = 0; j < nScanlines; ++j)
    {
        for (uint32 i = 0; i < scanlineWidth; ++i)
        {
            if (is.read((char*)&rgbe[0], 4).fail())
            {
                GLOG_ERROR << "HDR image flat pixels read error";
                return false;
            }
            rgbPixel.colour = RGB(rgbe);
            uint32 x = flippedX ? scanlineWidth-i-1 : i;
            uint32 y = flippedY ? nScanlines-j-1 : j;
            pixels[y*scanlineWidth + x] = rgbPixel;
        }
    }

    return true;
}

// HDR Codec
// This implementation is based on code by Bruce Walter.
// http://www.graphics.cornell.edu/~bjw/rgbe.html
bool ReadHdrHeaderLine(std::istream& is, char buffer[HdrMaxLineLength])
{
    memset(buffer, 0, sizeof(buffer));

    for (uint32 i = 0; i < HdrMaxLineLength; ++i)
    {
        if (is.read(&buffer[i], sizeof(buffer[i])).fail())
        {
            return false;
        }

        if (buffer[i] == '\n')
        {
            break;
        }
    }

    return true;
}

bool ReadHeader(std::istream& is, RGBEHeaderInfo& headerInfo, uint32& imgWidth, uint32& imgHeight, bool& flippedX, bool& flippedY)
{
    headerInfo.valid = 0;
    headerInfo.programType[0] = 0;
    headerInfo.gamma = headerInfo.exposure = 1.0f;

    char buffer[HdrMaxLineLength] = {0};

    if (!ReadHdrHeaderLine(is, buffer) || (buffer[0] != '#') || (buffer[1] != '?'))
    {
        return false;
    }

    headerInfo.valid |= rgbeValidProgramType;

    // Read the entire file signature
    uint32 i = 0;
    for (i = 0; i < sizeof(headerInfo.programType)-1; ++i)
    {
        if ((buffer[i+2] == 0) || isspace(buffer[i+2]))
        {
            break;
        }
        headerInfo.programType[i] = buffer[i+2];
    }
    headerInfo.programType[i] = 0;

    float tempf = 0;
    bool formatFound = false;
    bool headerFound = false;
    std::string format("FORMAT=32-bit_rle_rgbe");

    for (;;)
    {
        if (!ReadHdrHeaderLine(is, buffer))
        {
            GLOG_ERROR << "HDR header read error";
            return false;
        }

        if ((buffer[0] == 0) || (buffer[0] == '\n'))
        {
            headerFound = true;
            break;
        }
        else if (strncmp(buffer, format.c_str(), format.length()) == 0)
        {
            formatFound = true;
        }
        else if (sscanf(buffer, "GAMMA=%g", &tempf) == 1)
        {
            headerInfo.gamma = tempf;
            headerInfo.valid |= rgbeValidGamma;
        }
        else if (sscanf(buffer, "EXPOSURE=%g", &tempf) == 1)
        {
            headerInfo.exposure = tempf;
            headerInfo.valid |= rgbeValidExposure;
        }
        else if ((buffer[0] == '#') && (buffer[1] == 0x20))
        {
            strcpy(headerInfo.comment, buffer);
            headerInfo.valid |= rgbeValidComment;
        }
    }

    if (!headerFound || !formatFound)
    {
        GLOG_ERROR << "Invalid HDR header";
        return false;
    }

    if (!ReadHdrHeaderLine(is, buffer))
    {
        GLOG_ERROR << "HDR header read error";
        return false;
    }

    char ySign=0, xSign=0;
    if (sscanf(buffer, "%cY %d %cX %d", &ySign, &imgHeight, &xSign, &imgWidth) < 4)
    {
        GLOG_ERROR << "HDR header missing image size specifier";
        return false;
    }
    if (((ySign != '+') && (ySign != '-')) || ((xSign != '+') && (xSign != '-')))
    {
        GLOG_ERROR << "HDR header incorrect size specifier";
        return false;
    }

    flippedX = (xSign=='-');
    flippedY = (ySign=='+');

    return true;
}

bool ReadPixels(std::istream& is, RGBPixelList& pixels, uint32 scanlineWidth, uint32 nScanlines, bool flippedX, bool flippedY)
{
    {
        RGBPixelList(scanlineWidth*nScanlines).swap(pixels);
    }

    if ((scanlineWidth < 8) || (scanlineWidth > 0x7fff))
    {
        // Run-length encoding is not allowed, so read flat
        return ReadFlatRgbePixels(is, pixels, scanlineWidth, nScanlines, flippedX, flippedY);
    }

    RGBE rgbe;
    std::vector<byte> scanlineBuffer(4*scanlineWidth);

    // Read in each successive scanline
    for (uint32 iScanline = 0; iScanline < nScanlines; ++iScanline)
    {
        if (is.read((char*)&rgbe[0], sizeof(rgbe)).fail())
        {
            GLOG_ERROR << "HDR image data read error";
            return false;
        }

        if ((rgbe[0] != 2) || (rgbe[1] != 2) || (rgbe[2] & 0x80))
        {
            // This file is not run-length encoded, read flat
            if (is.seekg(-static_cast<int>(sizeof(rgbe)), std::ios_base::cur).fail())
            {
                GLOG_ERROR << "HDR image data read error";
                return false;
            }
            return ReadFlatRgbePixels(is, pixels, scanlineWidth, nScanlines, flippedX, flippedY);
        }

        if (((static_cast<uint32>(rgbe[2]) << 8) | static_cast<uint32>(rgbe[3])) != scanlineWidth)
        {
            GLOG_ERROR << "HDR image data wrong scanline width";
            return false;
        }

        // Read each of the four channels for the scanline into the buffer
        byte runCount = 0;
        for (uint32 i = 0; i < 4; ++i)
        {
            uint32 j = 0;
            while (j < scanlineWidth)
            {
                if (is.read((char*)&runCount, sizeof(runCount)).fail())
                {
                    GLOG_ERROR << "HDR image data read error";
                    return false;
                }

                if (runCount > 128) // A run of the same value
                {
                    runCount -= 128;
                    if ((runCount == 0) || (runCount > scanlineWidth-j))
                    {
                        GLOG_ERROR << "HDR image bad scanline data";
                        return false;
                    }

                    byte value = 0;
                    if (is.read((char*)&value, sizeof(value)).fail())
                    {
                        GLOG_ERROR << "HDR image data read error";
                        return false;
                    }

                    uint32 channelBegin = i*scanlineWidth;
                    while (runCount--)
                    {
                        scanlineBuffer[channelBegin + (j++)] = value;
                    }
                }
                else // Non run
                {
                    if ((runCount == 0) || (runCount > scanlineWidth-j))
                    {
                        GLOG_ERROR << "HDR image bad scanline data";
                        return false;
                    }
                    uint32 currPos = i*scanlineWidth+j;
                    if (is.read((char*)&scanlineBuffer[currPos], sizeof(byte)*runCount).fail())
                    {
                        GLOG_ERROR << "HDR image data read error";
                        return false;
                    }
                    j += runCount;
                }
            }
        }

        // Convert the data from the scanline buffer into float pixels
        RGBPixel rgbPixel(RGB::black);
        for (uint32 i = 0; i < scanlineWidth; ++i)
        {
            rgbe[0] = scanlineBuffer[i];
            rgbe[1] = scanlineBuffer[i + scanlineWidth];
            rgbe[2] = scanlineBuffer[i + 2*scanlineWidth];
            rgbe[3] = scanlineBuffer[i + 3*scanlineWidth];
            rgbPixel.colour = RGB(rgbe);

            uint32 x = flippedX ? scanlineWidth-i-1 : i;
            uint32 y = flippedY ? nScanlines-iScanline-1 : iScanline;
            pixels[y*scanlineWidth+x] = rgbPixel;
        }
    }

    return true;
}

bool WriteHeader(std::ostream& os, const RGBEHeaderInfo& headerInfo, uint32 imgWidth, uint32 imgHeight)
{
    std::ostringstream headerStr;
    std::string programType("RADIANCE");

    if (!(headerInfo.valid & rgbeValidProgramType))
    {
        programType = headerInfo.programType;
    }

    headerStr << "#?" << programType << "\n";

    if (headerInfo.valid & rgbeValidComment)
    {
        headerStr << "# " << headerInfo.comment << "\n";
    }

    headerStr << "FORMAT=32-bit_rle_rgbe\n";

    if (headerInfo.valid & rgbeValidGamma)
    {
        headerStr << "GAMMA=" << headerInfo.gamma << "\n";
    }

    if (headerInfo.valid & rgbeValidExposure)
    {
        headerStr << "EXPOSURE=" << headerInfo.exposure << "\n";
    }

    headerStr << "\n-Y " << imgHeight << " +X " << imgWidth << "\n";

    if (os.write(headerStr.str().c_str(), static_cast<int>(headerStr.str().length())).fail())
    {
        GLOG_ERROR << "HDR image header write error";
        return false;
    }

    return true;
}

bool WritePixels(std::ostream& os, const RGBPixelList& imgPixels)
{
    RGBE rgbe;
    size_t nPixels = imgPixels.size();

    for (size_t i = 0; i < nPixels; ++i)
    {
        rgbe = imgPixels[i].colour.ToRGBE();
        if (os.write((char*)&rgbe[0], sizeof(rgbe)).fail())
        {
            GLOG_ERROR << "HDR image data write error";
            return false;
        }
    }

    return true;
}

} // Anonymous namespace

namespace renderbliss
{
bool LoadHDR(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height)
{
    GLOG_INFO << "Loading HDR file " << fileName;

    int nBytesInFile = 0;
    std::vector<byte> filedata;

    if ((nBytesInFile = ReadFileIntoMemory(fileName, filedata)) <= 0)
    {
        GLOG_ERROR << "File " << fileName << ": failed to load HDR image.";
        return false;
    }

    RGBEHeaderInfo headerInfo;
    bool flippedX=false, flippedY=false;
    std::istrstream is((char*)&filedata[0], nBytesInFile);

    if (    ReadHeader(is, headerInfo, width, height, flippedX, flippedY)
        &&  ReadPixels(is, pixels, width, height, flippedX, flippedY))
    {
        return true;
    }
    GLOG_ERROR << "File " << fileName << ": failed to load HDR image.";
    return false;
}

bool SaveHDR(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height)
{
    GLOG_INFO << "Writing HDR file " << fileName;

    if (pixels.size() < width*height)
    {
        GLOG_ERROR << "File " << fileName << ": image size doesn't match width and height parameters.";
        GLOG_ERROR << "File " << fileName << ": failed to write HDR image.";
        return false;
    }

    RGBEHeaderInfo headerInfo;
    headerInfo.valid = 0;
    headerInfo.gamma = headerInfo.exposure = 1.0f;
    memset(headerInfo.comment,0,sizeof(headerInfo.comment));
    memset(headerInfo.programType,0,sizeof(headerInfo.programType));
    strncpy(headerInfo.programType, "RADIANCE", strlen("RADIANCE"));
    strncpy(headerInfo.comment, "Generated by RenderBliss", strlen("Generated by RenderBliss"));
    headerInfo.valid |= rgbeValidComment;
    headerInfo.valid |= rgbeValidProgramType;

    std::ofstream fout(fileName.c_str(), std::ios_base::binary | std::ios_base::trunc);

    if (!WriteHeader(fout, headerInfo, width, height) || !WritePixels(fout, pixels))
    {
        fout.close();
        GLOG_ERROR << "File " << fileName << ": failed to write HDR image.";
        return false;
    }

    fout.close();
    return true;
}
}
