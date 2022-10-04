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
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <png.h>
#include <log++/Log++.h>
#include "renderbliss/Types.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Utils/Utils.h"

#define PNG_READ_GRAY_TO_RGB_SUPPORTED

namespace
{
enum { PngBytesToCheck = 8 };

void PngReadFunction(png_structp pPng, png_bytep data, png_size_t length)
{
    std::ifstream* inputFileStream = static_cast<std::ifstream*>(png_get_io_ptr(pPng));
    if (!inputFileStream)
    {
        GLOG_ERROR << "Fatal error in PNG read function; can't continue.";
    }
    else if (inputFileStream->read((char*)data, static_cast<std::streamsize>(length)).fail())
    {
        GLOG_ERROR << "Fatal error in PNG read function; can't continue.";
    }
}

void PngWriteFunction(png_structp pPng, png_bytep data, png_size_t length)
{
    std::ofstream* outputFileStream = static_cast<std::ofstream*>(png_get_io_ptr(pPng));
    if (!outputFileStream)
    {
        GLOG_ERROR << "Fatal error in PNG write function; can't continue.";
    }
    else if (outputFileStream->write((char*)data, static_cast<std::streamsize>(length)).fail())
    {
        GLOG_ERROR << "Fatal error in PNG write function; can't continue.";
    }
}

void PngFlushFunction(png_structp pPng)
{
    RB_UNUSED(pPng);
}
}


namespace renderbliss
{
bool LoadPNG(const std::string& fileName, RGBPixelList& pixels, uint32& width, uint32& height)
{
    GLOG_INFO << "Loading PNG file " << fileName;

    std::ifstream inputFileStream(fileName.c_str(), std::ios_base::binary);

    // Verify that it is a PNG file by checking its signature
    byte pngSignature[PngBytesToCheck];
    if (inputFileStream.read((char*)&pngSignature[0], PngBytesToCheck).fail())
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to read PNG signature.";
        return false;
    }

    if (png_sig_cmp(pngSignature, 0, PngBytesToCheck))
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": invalid PNG signature.";
        return false;
    }

    png_structp pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pPng)
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": could not allocate PNG structure.";
        return false;
    }

    png_infop pPngInfo = png_create_info_struct(pPng);
    if (!pPngInfo)
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": could not allocate PNG info structure.";
        png_destroy_read_struct(&pPng, png_infopp_NULL, png_infopp_NULL);
        return false;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(pPng)))
    {
        png_destroy_read_struct(&pPng, &pPngInfo, png_infopp_NULL);
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to set PNG error handling.";
        return false;
    }

    png_set_read_fn(pPng, (png_voidp)(&inputFileStream), PngReadFunction);
    png_set_sig_bytes(pPng, PngBytesToCheck);

    // We now read the image using the libpng low-level functions

    png_read_info(pPng, pPngInfo);

    // Get the header information
    png_uint_32 w, h;
    int bitDepth, colorType, interlaceMethod;
    png_get_IHDR(pPng, pPngInfo, &w, &h, &bitDepth, &colorType, &interlaceMethod, NULL, NULL);
    width = static_cast<uint32>(w);
    height = static_cast<uint32>(h);

    // Strip 16-bit colors down to 8 bits
    if (bitDepth == 16)
    {
        png_set_strip_16(pPng);
    }

    // Expand paletted colors into true RGB triplets
    if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(pPng);
    }

    // Expand grayscale images to full 8bits/pixel
    if ((colorType == PNG_COLOR_TYPE_GRAY) && (bitDepth < 8))
    {
        png_set_gray_1_2_4_to_8(pPng);
    }

    // Expand grayscale images into RGB images
    if ((colorType == PNG_COLOR_TYPE_GRAY) || (colorType == PNG_COLOR_TYPE_GRAY_ALPHA))
    {
        png_set_gray_to_rgb(pPng);
    }

    // Expand images with transparency to full alpha channels
    if (png_get_valid(pPng, pPngInfo, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(pPng);
    }

    // Update the image information, since we've applied some transformations
    png_read_update_info(pPng, pPngInfo);
    colorType = png_get_color_type(pPng, pPngInfo);

    // Allocate row buffers to hold image data
    boost::scoped_array<png_bytep> rowPointers(new png_bytep[height]);
    for (uint32 j = 0; j < height; ++j)
    {
        rowPointers[j] = (png_bytep)png_malloc(pPng, png_get_rowbytes(pPng, pPngInfo));
    }

    // Read the entire image at once
    png_read_image(pPng, &rowPointers[0]);

    // Fill the pixels array
    RGBPixel rgbPixel(RGB::black);
    pixels.clear();
    pixels.reserve(static_cast<size_t>(width*height));
    for (uint32 j = 0; j < height; ++j)
    {
        png_bytep currentRow = rowPointers[j];
        for (uint32 i = 0; i < width; ++i)
        {
            png_bytep currentColor = (colorType == PNG_COLOR_TYPE_RGBA) ? &currentRow[i*4] : &currentRow[i*3];
            rgbPixel.colour.r = currentColor[0]*Inv255();
            rgbPixel.colour.g = currentColor[1]*Inv255();
            rgbPixel.colour.b = currentColor[2]*Inv255();
            if (colorType == PNG_COLOR_TYPE_RGBA)
            {
                rgbPixel.opacity = currentColor[3]*Inv255();
            }
            pixels.push_back(rgbPixel);
        }
    }
    png_read_end(pPng, pPngInfo);

    // Free allocated memory
    for (uint32 j = 0; j < height; ++j)
    {
        png_free(pPng, rowPointers[j]);
    }
    png_destroy_read_struct(&pPng, &pPngInfo, png_infopp_NULL);

    return true;
}


bool SavePNG(const std::string& fileName, const RGBPixelList& pixels, uint32 width, uint32 height)
{
    GLOG_INFO << "Writing PNG file " << fileName;

    if (pixels.size() < width*height)
    {
        return false;
    }

    // Allocate the PNG write structure
    png_structp pPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pPng)
    {
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to allocate PNG write structure.";
        return false;
    }

    // Allocate image information data
    png_infop pPngInfo = png_create_info_struct(pPng);
    if (!pPngInfo)
    {
        png_destroy_write_struct(&pPng, png_infopp_NULL);
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to allocate PNG info structure.";
        return false;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(pPng)))
    {
        png_destroy_write_struct(&pPng, &pPngInfo);
        GLOG_ERROR << "Error occurred while loading " << fileName << ": failed to set PNG error handling.";
        return false;
    }

    // Set write function
    std::ofstream outputFileStream(fileName.c_str(), std::ios_base::binary);
    png_set_write_fn(pPng, (png_voidp)(&outputFileStream), PngWriteFunction, PngFlushFunction);

    // Set header information
    png_set_IHDR(pPng, pPngInfo, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Don't apply filters
    png_set_filter(pPng, 0, PNG_NO_FILTERS);

    // No gamma correction
    png_set_gAMA(pPng, pPngInfo, 1.0f);

    // Build rows
    RGBPixelList::const_iterator currentPixelIter = pixels.begin();
    boost::scoped_array<png_bytep> rowPointers(new png_bytep[height]);
    for (uint32 j = 0; j < height; ++j)
    {
        rowPointers[j] = (png_bytep)png_malloc(pPng, 4*width);
        png_bytep currentRow = rowPointers[j];
        for (uint32 i = 0; i < width; ++i, ++currentPixelIter)
        {
            png_bytep currentColour = &currentRow[i*4];
            currentColour[0] = static_cast<png_byte>(255.0f*currentPixelIter->colour.r);
            currentColour[1] = static_cast<png_byte>(255.0f*currentPixelIter->colour.g);
            currentColour[2] = static_cast<png_byte>(255.0f*currentPixelIter->colour.b);
            currentColour[3] = static_cast<png_byte>(255.0f*currentPixelIter->opacity);
        }
    }

    // Put the image data in the info structure
    png_set_rows(pPng, pPngInfo, &rowPointers[0]);

    // Write out the image
    png_write_png(pPng, pPngInfo, PNG_TRANSFORM_IDENTITY, NULL);

    // Free allocated memory
    for (uint32 j = 0; j < height; ++j)
    {
        png_free(pPng, rowPointers[j]);
    }
    png_destroy_write_struct(&pPng, &pPngInfo);

    return true;
}
}
