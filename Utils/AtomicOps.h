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

#ifndef RENDERBLISS_ATOMIC_OPS_H
#define RENDERBLISS_ATOMIC_OPS_H

#include "renderbliss/Types.h"

namespace renderbliss
{
// Atomically adds a delta to a 32-bit integer in memory and returns its previous value.
uint32 AtomicAdd(volatile uint32* value, uint32 delta);

// Atomically adds a delta to a single precision floating point in memory and returns its previous value.
float AtomicAdd(volatile float* value, float delta);

// Atomically increments a 32-bit integer in memory and returns its previous value.
uint32 AtomicIncrement(volatile uint32* value);

// Helper class for atomic integral count operations
class AtomicCounter
{
public:

    AtomicCounter();
    void Add(uint32 delta);
    void operator++();
    void operator++(int);
    operator uint32() const volatile;

private:

    volatile uint32 count;
};
}

#endif
