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

#ifndef RENDERBLISS_UTILS_H
#define RENDERBLISS_UTILS_H

#include <string>
#include <vector>
#include "renderbliss/Types.h"

namespace renderbliss
{
template <typename T>
bool FlipArrayVertically(uint32 arrayWidth, uint32 arrayHeight, std::vector<T>& arrayData);

template <typename T>
bool FlipArrayHorizontally(uint32 arrayWidth, uint32 arrayHeight, std::vector<T>& arrayData);

// Returns the number of hardware thread counts (e.g. number of core or hyperthreading units)
unsigned HardwareThreadCount();

// Reads a file into an array of bytes. Returns the number of bytes read, or -1 if an error occurs.
int ReadFileIntoMemory(const std::string& fileName, std::vector<byte>& filedata);
}

#include "renderbliss/Utils/Utils.inl"

#endif
