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

#include "renderbliss/Utils/StatsTracker.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <log++/Log++.h>
#include "renderbliss/Macros.h"

namespace renderbliss
{
StatsTracker::StatsTracker()
{
}

void StatsTracker::AddCounter(const std::string& category, const std::string& name)
{
    counterCategories[category][name];
}

void StatsTracker::AddTimer(const std::string& category, const std::string& name)
{
    timerCategories[category][name];
}

AtomicCounter& StatsTracker::Counter(const std::string& category, const std::string& name)
{
    return counterCategories[category][name];
}

const AtomicCounter& StatsTracker::Counter(const std::string& category, const std::string& name) const
{
    return counterCategories.at(category).at(name);
}

void StatsTracker::Log() const
{
    foreach (const CounterCategoryMap::value_type& counterCategory, counterCategories)
    {
        const CounterMap& counters = counterCategory.second;
        foreach (const CounterMap::value_type& pair, counters)
        {
            GLOG_INFO << pair.first << ": " << pair.second;
        }
    }

    foreach (const TimerCategoryMap::value_type& timerCategory, timerCategories)
    {
        const TimerMap& timers = timerCategory.second;
        foreach (const TimerMap::value_type& pair, timers)
        {
            using namespace boost::posix_time;
            time_duration td = seconds(static_cast<long>(pair.second.Elapsed()));
            GLOG_INFO << pair.first << ": " << to_simple_string(td);
        }
    }
}

Timer& StatsTracker::Timer(const std::string& category, const std::string& name)
{
    return timerCategories[category][name];
}

const Timer& StatsTracker::Timer(const std::string& category, const std::string& name) const
{
    return timerCategories.at(category).at(name);
}
}
