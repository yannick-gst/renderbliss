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

#ifndef RENDERBLISS_MERSENNE_TWISTER_H
#define RENDERBLISS_MERSENNE_TWISTER_H

#include <ctime>
#include <boost/random.hpp>
#include "renderbliss/Types.h"

namespace renderbliss
{
struct Vector2;
struct Vector3;

// A pseudorandom number generator
class MersenneTwister
{
public:

    MersenneTwister(uint seed = static_cast<uint>(std::time(0)));
    // Generates a real number in the range [0,1)
    real    CanonicalRandom();
    // Generates a 2d vector of numbers in the range [0,1)
    Vector2 CanonicalRandom2();
    // Generates a 3d vector of numbers in the range [0,1)
    Vector3 CanonicalRandom3();
    // Generates an unsigned integer in the range [0,max]
    uint RandomUint(uint max);
    // Generates an unsigned integer in the range [min,max]
    uint RandomUint(uint min, uint max);
    // Generates an real in the range [min,max)
    real RandomReal(real min, real max);

private:

    boost::uniform_01<boost::mt19937, real> realGenerator;
};

template <typename ValueType>
void Shuffle(MersenneTwister& rng, std::vector<ValueType>& values)
{
    if (values.empty()) return;
    Shuffle(rng, &values[0], values.size());
}

template <typename ValueType>
void Shuffle(MersenneTwister& rng, ValueType* values, size_t nValues)
{
    if (!values || !nValues) return;
    for (size_t i = nValues-1; i > 0; --i)
    {
        uint32 target = rng.RandomUint(i);
        std::swap(values[target], values[i]);
    }
}
}

#endif
