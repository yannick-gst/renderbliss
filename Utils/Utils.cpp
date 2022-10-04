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

#include "renderbliss/Utils/Utils.h"
#include <algorithm>
#include <fstream>
#include <boost/thread.hpp>

namespace renderbliss
{
unsigned HardwareThreadCount()
{
    return std::max(static_cast<unsigned>(1), boost::thread::hardware_concurrency());
}

int ReadFileIntoMemory(const std::string& fileName, std::vector<byte>& filedata)
{
    std::ifstream inputFile(fileName.c_str(), std::ios::binary);
    if (!inputFile.fail())
    {
        inputFile.seekg(0, std::ios::end);
        int nBytes = inputFile.tellg();
        if (nBytes > 0)
        {
            inputFile.seekg(0, std::ios_base::beg);
            std::vector<byte>(nBytes).swap(filedata);
            inputFile.read((char*)&filedata[0], nBytes);
            if (inputFile.fail())
            {
                return -1;
            }
        }
        return nBytes;
    }
    return -1;
}
}
