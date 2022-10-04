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

#ifndef RENDERBLISS_BLOCKEDARRAY_H
#define RENDERBLISS_BLOCKEDARRAY_H

#include <vector>
#include <boost/array.hpp>

namespace renderbliss
{
// Thic class represents a 2D array subdivided into square blocks of fixed power of two size for better
// memory coherence in scenarios such as texture mapping or image reconstruction. Idea borrowed from PBRT.
template <typename ItemType>
class BlockedArray
{
public:

    BlockedArray(size_t width, size_t height);
    BlockedArray(const std::vector<ItemType>& arrayData, size_t arrayWidth, size_t arrayHeight);

    void CopyToLinearArray(std::vector<ItemType>& linearArray) const;
    size_t Height() const;
    size_t Width() const;

    ItemType& operator()(size_t i, size_t j);
    const ItemType& operator()(size_t i, size_t j) const;

private:

    enum { LogBlockSize = 2 };

    // Represents a square block in the blocked array
    template <typename T>
    struct Block
    {
        boost::array<T, (1<<LogBlockSize)*(1<<LogBlockSize)> blockData;
    };

    std::vector< Block<ItemType> > blocks;
    size_t width, height;
    size_t blocklineWidth;

    size_t BlockSize() const;
    size_t RoundToBlockSizeMultiple(size_t a) const;
};

}

#include "renderbliss/Utils/BlockedArray.inl"

#endif
