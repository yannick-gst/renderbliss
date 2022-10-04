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

#ifndef RENDERBLISS_FILTER_EXTENTS_CHECK_MACRO_H
#define RENDERBLISS_FILTER_EXTENTS_CHECK_MACRO_H

#include <cmath>
#include "renderbliss/Macros.h"

#ifndef NDEBUG
#define FILTER_EXTENTS_CHECK_ENABLED 1
#else
#define FILTER_EXTENTS_CHECK_ENABLED 0
#endif

#if FILTER_EXTENTS_CHECK_ENABLED
    #define CHECK_AGAINST_FILTER_EXTENTS(x, y) \
        MULTI_LINE_MACRO_BEGIN \
            if ((fabs(x) > XWidth()) || (fabs(y) > YWidth())) \
            { \
                return 0.0f; \
            } \
        MULTI_LINE_MACRO_END
#else
    #define CHECK_AGAINST_FILTER_EXTENTS(x,y) \
        MULTI_LINE_MACRO_BEGIN \
            RB_UNUSED(x); \
            RB_UNUSED(y); \
        MULTI_LINE_MACRO_END
#endif

#endif
