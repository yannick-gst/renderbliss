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

#include <algorithm>

namespace renderbliss
{
template <typename T>
bool FlipArrayVertically(uint32 arrayWidth, uint32 arrayHeight, std::vector<T>& arrayData)
{
    if (arrayData.size() >= arrayWidth*arrayHeight)
    {
        for (uint32 j = 0; j < arrayHeight/2; ++j)
        {
            for (uint32 i = 0; i < arrayWidth; ++i)
            {
                std::swap(arrayData[j*arrayWidth + i], arrayData[(arrayHeight-j-1)*arrayWidth + i]);
            }
        }
        return true;
    }
    return false;
}

template <typename T>
bool FlipArrayHorizontally(uint32 arrayWidth, uint32 arrayHeight, std::vector<T>& arrayData)
{
    if (arrayData.size() >= arrayWidth*arrayHeight)
    {
        for (uint32 j = 0; j < arrayHeight; ++j)
        {
            for (uint32 i = 0; i < arrayWidth/2; ++i)
            {
                std::swap(arrayData[j*arrayWidth + i], arrayData[j*arrayWidth + (arrayWidth-i-1)]);
            }
        }
        return true;
    }
    return false;
}
}
