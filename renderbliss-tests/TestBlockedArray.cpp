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

#include <vector>
#include <UnitTest++.h>
#include "renderbliss/Utils/BlockedArray.h"

namespace
{
using namespace renderbliss;

TEST(CheckBuildFromLinearArray)
{
    size_t w = 127;
    size_t h = 253;
    size_t N = w*h;

    std::vector<size_t> linearArray;
    for (size_t i = 0; i < N; ++i)
    {
        linearArray.push_back(i);
    }

    BlockedArray<size_t> blockedArray(linearArray, w, h);

    std::vector<size_t>::const_iterator linearIter = linearArray.begin();
    for (size_t j = 0; j < blockedArray.Height(); ++j)
    {
        for (size_t i = 0; i < blockedArray.Width(); ++i)
        {
            CHECK_EQUAL(*(linearIter++), blockedArray(i,j));
        }
    }
}

TEST(CheckCopyToLinearArray)
{
    size_t w = 127;
    size_t h = 253;
    size_t N = w*h;

    std::vector<size_t> linearArray;
    for (size_t i = 0; i < N; ++i)
    {
        linearArray.push_back(i);
    }

    BlockedArray<size_t> blockedArray(linearArray, w, h);

    std::vector<size_t> linearArray2;
    blockedArray.CopyToLinearArray(linearArray2);

    CHECK_EQUAL(linearArray.size(), linearArray2.size());

    for (size_t i = 0; i < linearArray.size(); ++i)
    {
        CHECK_EQUAL(linearArray[i], linearArray2[i]);
    }
}
}
