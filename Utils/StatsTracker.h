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

#ifndef RENDERBLISS_STATS_TRACKER_H
#define RENDERBLISS_STATS_TRACKER_H

#include <string>
#include <boost/unordered_map.hpp>
#include "renderbliss/Utils/AtomicOps.h"
#include "renderbliss/Utils/Timer.h"

namespace renderbliss
{
// A helper class to track time and count statistics
class StatsTracker
{
public:

    StatsTracker();
    void AddCounter(const std::string& category, const std::string& name);
    void AddTimer(const std::string& category, const std::string& name);
    AtomicCounter& Counter(const std::string& category, const std::string& name);
    const AtomicCounter& Counter(const std::string& category, const std::string& name) const;
    void Log() const;
    Timer& Timer(const std::string& category, const std::string& name);
    const renderbliss::Timer& Timer(const std::string& category, const std::string& name) const;

private:

    typedef boost::unordered_map<std::string/*name*/, AtomicCounter> CounterMap;
    typedef boost::unordered_map<std::string/*name*/, renderbliss::Timer> TimerMap;
    typedef boost::unordered_map<std::string/*category*/, CounterMap> CounterCategoryMap;
    typedef boost::unordered_map<std::string/*category*/, TimerMap> TimerCategoryMap;
    CounterCategoryMap counterCategories;
    TimerCategoryMap timerCategories;
};
}

#endif
