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

#include "renderbliss/Utils/JobScheduler.h"
#include <deque>
#include <boost/thread.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Interfaces/IJob.h"
#include "renderbliss/Utils/Utils.h"

namespace renderbliss
{
// A mutex protected job queue for parallel processing
class JobQueue
{
public:

    JobQueue();
    // Adds a job to the queue. Should be called as part of a preprocessing step before all jobs are run.
    void Enqueue(const JobConstPtr& job);
    // Adds jobs to the queue. Should be called as part of a preprocessing step before all jobs are run.
    void Enqueue(const JobList& jobs);
    // Fetches and returns a job from the queue. Returns a null job if the queue is empty.
    JobConstPtr Next();

private:

    mutable boost::mutex mutex;
    std::deque<JobConstPtr> jobs;
};

// Thread running jobs fetched from a job queue
class SchedulerThread
{
public:

    SchedulerThread(const boost::shared_ptr<JobQueue>& jobQueue);
    void operator()();

private:

    boost::shared_ptr<JobQueue> jobQueue;
};
}

namespace renderbliss
{
JobQueue::JobQueue()
{
}

void JobQueue::Enqueue(const JobConstPtr& job)
{
    boost::mutex::scoped_lock lock(mutex);
    if (job) jobs.push_back(job);
}

void JobQueue::Enqueue(const JobList& jobs)
{
    boost::mutex::scoped_lock lock(mutex);
    foreach (const JobConstPtr& j, jobs)
    {
        if (j) this->jobs.push_back(j);
    }
}

JobConstPtr JobQueue::Next()
{
    JobConstPtr result;
    {
        boost::mutex::scoped_lock lock(mutex);
        if (!jobs.empty())
        {
            result = jobs.front();
            jobs.pop_front();
        }
    }
    return result;
}

SchedulerThread::SchedulerThread(const boost::shared_ptr<JobQueue>& jobQueue) : jobQueue(jobQueue)
{
}

void SchedulerThread::operator()()
{
    while (JobConstPtr j = jobQueue->Next())
    {
        j->Run();
    }
}

const unsigned JobScheduler::nThreads = HardwareThreadCount();

JobScheduler::JobScheduler()
{
    jobQueue.reset(new JobQueue);
}

void JobScheduler::Spawn(const JobConstPtr& job)
{
    if (!job) return;
    (nThreads == 1) ? job->Run() : jobQueue->Enqueue(job);
}

void JobScheduler::Spawn(const JobList& jobs)
{
    if (jobs.empty()) return;

    if (nThreads == 1)
    {
        foreach (const JobConstPtr& j, jobs)
        {
            j->Run();
        }
    }
    else
    {
        jobQueue->Enqueue(jobs);
    }
}

void JobScheduler::WaitForAllJobs()
{
    if (nThreads > 1)
    {
        boost::thread_group threads;
        for (unsigned i = 0; i < nThreads; ++i)
        {
            threads.add_thread(new boost::thread(SchedulerThread(jobQueue)));
        }
        threads.join_all();
    }
}
}
