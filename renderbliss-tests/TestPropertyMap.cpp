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
#include "renderbliss/Types.h"
#include "renderbliss/Utils/PropertyMap.h"

namespace
{
using namespace renderbliss;
TEST(CheckGetAndSet)
{
    uint32 v = 0;
    PropertyMap pm;

    pm.Get<uint32>("p1", 4, v);
    CHECK(v==4);

    pm.Set<uint32>("p1", 1);
    pm.Get<uint32>("p1", 4, v);
    CHECK(v==1);

    std::string s;
    pm.Get<std::string>("p1", "default", s);
    CHECK(s=="default");

    PropertyMap pm2;
    pm2.SetFrom(pm);
    pm2.Get<uint32>("p1", 4, v);
    CHECK(v==1);
}
}
