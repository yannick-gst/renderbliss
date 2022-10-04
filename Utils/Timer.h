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

#ifndef RENDERBLISS_TIMER_H
#define RENDERBLISS_TIMER_H

#include <iosfwd>
#include <string>
#include <boost/timer.hpp>

namespace renderbliss
{
// Platform independent timer class
// Usage example:
//   Timer t;
//   t.Start();
//   ... (instructions to be timed go here)
//   ... (instructions to be timed go here)
//   ... (instructions to be timed go here)
//   t.Stop();
//   double elapsedTime = t.Elapsed();
class Timer
{
public:

    Timer(const std::string& label="");
    void Start();
    void Stop();
    double Elapsed() const; // Returns the elapsed time in seconds
    const std::string& Label() const;

private:

    std::string label;
    boost::timer timer;
    double elapsed;
};

// Streams the elapsed time
std::ostream& operator << (std::ostream& os, const Timer& timer);
}

#endif
