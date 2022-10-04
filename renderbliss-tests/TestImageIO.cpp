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

#include <UnitTest++.h>
#include "renderbliss/ImageIO/ImageIO.h"

namespace
{
using namespace renderbliss;

TEST(CheckEXR)
{
    RGBPixelList pixelsRead;
    uint32 width=0, height=0;
    CHECK(LoadEXR("nyc-hdr.exr", pixelsRead, width, height));

    RGBPixelList pixelsWritten;
    CHECK(SaveImage("nyc-hdr-w.exr", pixelsRead, width, height));
    CHECK(LoadEXR("nyc-hdr-w.exr", pixelsWritten, width, height));

    CHECK_EQUAL(pixelsRead.size(), pixelsWritten.size());

    for (size_t i = 0; i < pixelsRead.size(); ++i)
    {
        CHECK_CLOSE(pixelsRead[i].colour.r, pixelsWritten[i].colour.r, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.g, pixelsWritten[i].colour.g, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.b, pixelsWritten[i].colour.b, 0.0001f);
    }
}

TEST(CheckHDR)
{
    RGBPixelList pixelsRead;
    uint32 width=0, height=0;
    CHECK(LoadHDR("nyc-hdr.hdr", pixelsRead, width, height));

    RGBPixelList pixelsWritten;
    CHECK(SaveImage("nyc-hdr-w.hdr", pixelsRead, width, height));
    CHECK(LoadHDR("nyc-hdr-w.hdr", pixelsWritten, width, height));

    CHECK_EQUAL(pixelsRead.size(), pixelsWritten.size());

    for (size_t i = 0; i < pixelsRead.size(); ++i)
    {
        CHECK_CLOSE(pixelsRead[i].colour.r, pixelsWritten[i].colour.r, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.g, pixelsWritten[i].colour.g, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.b, pixelsWritten[i].colour.b, 0.0001f);
    }
}

TEST(CheckPNG)
{
    RGBPixelList pixelsRead;
    uint32 width=0, height=0;
    CHECK(LoadPNG("nyc-hdr.png", pixelsRead, width, height));

    RGBPixelList pixelsWritten;
    CHECK(SaveImage("nyc-hdr-w.png", pixelsRead, width, height));
    CHECK(LoadPNG("nyc-hdr-w.png", pixelsWritten, width, height));

    CHECK_EQUAL(pixelsRead.size(), pixelsWritten.size());

    for (size_t i = 0; i < pixelsRead.size(); ++i)
    {
        CHECK_CLOSE(pixelsRead[i].colour.r, pixelsWritten[i].colour.r, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.g, pixelsWritten[i].colour.g, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.b, pixelsWritten[i].colour.b, 0.0001f);
    }
}

TEST(CheckTGA)
{
    RGBPixelList pixelsRead;
    uint32 width=0, height=0;
    CHECK(LoadTGA("nyc-hdr.tga", pixelsRead, width, height));

    RGBPixelList pixelsWritten;
    CHECK(SaveImage("nyc-hdr-w.tga", pixelsRead, width, height));
    CHECK(LoadTGA("nyc-hdr-w.tga", pixelsWritten, width, height));

    CHECK_EQUAL(pixelsRead.size(), pixelsWritten.size());

    for (size_t i = 0; i < pixelsRead.size(); ++i)
    {
        CHECK_CLOSE(pixelsRead[i].colour.r, pixelsWritten[i].colour.r, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.g, pixelsWritten[i].colour.g, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.b, pixelsWritten[i].colour.b, 0.0001f);
    }
}

TEST(CheckRunLengthEncodedTGA)
{
    RGBPixelList pixelsRead;
    uint32 width=0, height=0;
    CHECK(LoadTGA("nyc-hdr-rle.tga", pixelsRead, width, height));

    RGBPixelList pixelsWritten;
    CHECK(SaveImage("nyc-hdr-rle-w.tga", pixelsRead, width, height));
    CHECK(LoadTGA("nyc-hdr-rle-w.tga", pixelsWritten, width, height));

    CHECK_EQUAL(pixelsRead.size(), pixelsWritten.size());

    for (size_t i = 0; i < pixelsRead.size(); ++i)
    {
        CHECK_CLOSE(pixelsRead[i].colour.r, pixelsWritten[i].colour.r, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.g, pixelsWritten[i].colour.g, 0.0001f);
        CHECK_CLOSE(pixelsRead[i].colour.b, pixelsWritten[i].colour.b, 0.0001f);
    }
}
}
