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

#include "renderbliss/Utils/AtomicOps.h"
#include <boost/interprocess/detail/atomic.hpp>
#include "renderbliss/Macros.h"
#include "renderbliss/Types.h"

namespace renderbliss
{
// FIXME The usage of this namespace is questionable, as it
// is not part of the Boost public API. It would be advisable
// to consider an atomic library. Candidates include the atomic
// library shipping with Intel's TBB, or HP's atomic_ops library.
using namespace boost::interprocess::ipcdetail;

uint32 AtomicAdd(volatile uint32* value, uint32 delta)
{
    RB_ASSERT(value);
    uint32 oldValue, newValue;
    do
    {
        oldValue = *value;
        newValue = oldValue + delta;
    } while(atomic_cas32(value, newValue, oldValue) != oldValue);
    return newValue;
}

float AtomicAdd(volatile float* value, float delta)
{
    RB_ASSERT(value);
    volatile uint32* v = reinterpret_cast<volatile uint32*>(value);
    union {float f; uint32 i;} oldValue, newValue;
    do
    {
        oldValue.f = *value;
        newValue.f = oldValue.f + delta;
    } while (atomic_cas32(v, newValue.i, oldValue.i) != oldValue.i);
    return newValue.f;
}

uint32 AtomicIncrement(volatile uint32* value)
{
    RB_ASSERT(value);
    return atomic_inc32(value);
}

AtomicCounter::AtomicCounter() : count(0)
{
}

void AtomicCounter::Add(uint32 delta)
{
    AtomicAdd(&count, delta);
}

void AtomicCounter::operator++()
{
    AtomicIncrement(&count);
}

void AtomicCounter::operator++(int)
{
    AtomicIncrement(&count);
}

AtomicCounter::operator uint32() const volatile
{
    return count;
}
}
