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

#ifndef RENDERBLISS_JOB_SCHEDULER_H
#define RENDERBLISS_JOB_SCHEDULER_H

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace renderbliss
{
class IJob;
class JobQueue;
typedef boost::shared_ptr<const IJob> JobConstPtr;
typedef std::vector<JobConstPtr> JobList;
// A scheduler responsible for spawning and running concurrent jobs
class JobScheduler : boost::noncopyable
{
public:

    JobScheduler();

    // Schedules a job for execution.
    // The job is run immediately if there is only one hardware thread in the system.
    void Spawn(const JobConstPtr& jobs);

    // Schedules jobs for execution.
    // The jobs are run immediately if there is only one hardware thread in the system.
    void Spawn(const JobList& jobs);

    // Waits for all jobs to finish
    void WaitForAllJobs();

private:

    boost::shared_ptr<JobQueue> jobQueue;
    static const unsigned nThreads;
};
}

#endif
