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

#include <cassert>
#include "renderbliss/Macros.h"
#include "renderbliss/Math/MathUtils.h"

namespace renderbliss
{
template <typename ItemType>
BlockedArray<ItemType>::BlockedArray(size_t width, size_t height) : width(width), height(height)
{
    if ((Width() == 0) || (Height() == 0))
    {
        blocklineWidth = 0;
    }
    else
    {
        size_t nItemsPerBlock = Sqr(BlockSize());
        size_t roundedWidth = RoundToBlockSizeMultiple(Width());
        size_t roundedHeight = RoundToBlockSizeMultiple(Height());
        size_t nBlocks = roundedWidth*roundedHeight/nItemsPerBlock;

        blocklineWidth = roundedWidth >> LogBlockSize;
        std::vector< Block<ItemType> >(nBlocks).swap(blocks);
    }
}

template <typename ItemType>
BlockedArray<ItemType>::BlockedArray(const std::vector<ItemType>& arrayData, size_t arrayWidth, size_t arrayHeight)
{
    if (arrayData.empty() || (arrayWidth*arrayHeight > arrayData.size()))
    {
        width = height = blocklineWidth = 0;
    }
    else
    {
        width = arrayWidth;
        height = arrayHeight;
        size_t nItemsPerBlock = Sqr(BlockSize());
        size_t roundedWidth = RoundToBlockSizeMultiple(Width());
        size_t roundedHeight = RoundToBlockSizeMultiple(Height());
        size_t nBlocks = roundedWidth*roundedHeight/nItemsPerBlock;
        blocklineWidth = roundedWidth >> LogBlockSize;

        // Fill the blocked array
        std::vector< Block<ItemType> >(nBlocks).swap(blocks);
        for (size_t j = 0; j < Height(); ++j)
        {
            for (size_t i = 0; i < Width(); ++i)
            {
                size_t arrayIndex = j*Width() + i;
                operator()(i,j) = arrayData[arrayIndex];
            }
        }
    }
}

template <typename ItemType>
size_t BlockedArray<ItemType>::Width() const
{
    return width;
}

template <typename ItemType>
size_t BlockedArray<ItemType>::Height() const
{
    return height;
}

template <typename ItemType>
ItemType& BlockedArray<ItemType>::operator()(size_t i, size_t j)
{
    RB_ASSERT(i < Width());
    RB_ASSERT(j < Height());
    size_t blockIndex = (j>>LogBlockSize)*blocklineWidth + (i >> LogBlockSize);
    size_t itemIndex  = BlockSize()*(j & (BlockSize()-1)) + (i & (BlockSize()-1));
    return blocks[blockIndex].blockData[itemIndex];
}

template <typename ItemType>
const ItemType& BlockedArray<ItemType>::operator()(size_t i, size_t j) const
{
    RB_ASSERT(i < Width());
    RB_ASSERT(j < Height());
    size_t blockIndex = (j>>LogBlockSize)*blocklineWidth + (i >> LogBlockSize);
    size_t itemIndex  = BlockSize()*(j & (BlockSize()-1)) + (i & (BlockSize()-1));
    return blocks[blockIndex].blockData[itemIndex];
}

template <typename ItemType>
void BlockedArray<ItemType>::CopyToLinearArray(std::vector<ItemType>& linearArray) const
{
    std::vector<ItemType>(Width()*Height()).swap(linearArray);
    for (size_t j = 0; j < Height(); ++j)
    {
        for (size_t i = 0; i < Width(); ++i)
        {
            linearArray[j*Width() + i] = operator()(i,j);
        }
    }
}

template <typename ItemType>
size_t BlockedArray<ItemType>::BlockSize() const
{
    return 1 << LogBlockSize;
}

template <typename ItemType>
size_t BlockedArray<ItemType>::RoundToBlockSizeMultiple(size_t a) const
{
    return (a + BlockSize() - 1) & ~(BlockSize() - 1);
}
}
