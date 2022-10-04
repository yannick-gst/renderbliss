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

#ifndef RENDERBLISS_IRENDERINGJOB_H
#define RENDERBLISS_IRENDERINGJOB_H

#include "renderbliss/Types.h"
#include "renderbliss/Interfaces/IJob.h"

namespace renderbliss
{
// A data structure representing a decomposition of pixels for scheduling rendering work
struct RenderingWorkArea
{
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
};

// Abstract rendering job class for parallel rendering
class IRenderingJob : public IJob
{
public:

    IRenderingJob(const RenderingWorkArea& rwa) : workArea(rwa) {}
    virtual ~IRenderingJob() {}

protected:

    RenderingWorkArea workArea;
};
}

#endif
