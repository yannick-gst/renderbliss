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

#include "renderbliss/Rendering/Renderer.h"
#include <cstdlib>
#include "renderbliss/Macros.h"
#include "renderbliss/Scene.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Interfaces/ICamera.h"
#include "renderbliss/Interfaces/IFilm.h"
#include "renderbliss/Interfaces/IRenderingJob.h"
#include "renderbliss/Integrators/SurfaceIntegrator.h"
#include "renderbliss/Math/MersenneTwister.h"
#include "renderbliss/Math/Geometry/Ray.h"
#include "renderbliss/Utils/JobScheduler.h"
#include "renderbliss/Utils/PropertyMap.h"
#include "renderbliss/Utils/StatsTracker.h"

namespace renderbliss
{
// Default rendering job class
class RenderingJob : public IRenderingJob
{
public:

    RenderingJob(uint seed, const RenderingWorkArea& workArea, const ICamera* camera,
                 const Scene* scene, const SurfaceIntegrator* surfaceIntegrator);
    virtual void Run() const;

private:

    mutable MersenneTwister rng;
    const ICamera* camera;
    const Scene* scene;
    const SurfaceIntegrator* surfaceIntegrator;
};
}

namespace renderbliss
{
RenderingJob::RenderingJob(uint seed, const RenderingWorkArea& workArea, const ICamera* camera,
                           const Scene* scene, const SurfaceIntegrator* surfaceIntegrator)
    : IRenderingJob(workArea), rng(seed), camera(camera), scene(scene), surfaceIntegrator(surfaceIntegrator)
{
    RB_ASSERT(camera);
    RB_ASSERT(scene);
    RB_ASSERT(surfaceIntegrator);
}

void RenderingJob::Run() const
{
    Ray ray;
    CameraSample cs;
    uint32 nSamplesPerPixel = camera->SamplesPerPixel();
    for (int y = workArea.yStart; y <= workArea.yEnd; ++y)
    {
        for (int x = workArea.xStart; x <= workArea.xEnd; ++x)
        {
            camera->GeneratePixelSamples(x, y, rng, cs);
            for (uint32 iSample = 0; iSample < nSamplesPerPixel; ++iSample)
            {
                RB_ASSERT(iSample < cs.imageSamples.size());
                RB_ASSERT(iSample < cs.lensSamples.size());
                RB_ASSERT(iSample < cs.timeSamples.size());
                PixelSample ps = {cs.imageSamples[iSample], cs.lensSamples[iSample], cs.timeSamples[iSample]};
                camera->GenerateRay(ps, ray);
                real opacity = 1.0f;
                FilmSample s = { surfaceIntegrator->Radiance(*scene, ray, rng, opacity).ToXYZ(),
                                 ps.imageSample,
                                 1.0f };
                camera->Film()->AddSample(s);
            }
        }
    }
}

Renderer::Settings::Settings(const PropertyMap&)
{
}

Renderer::Renderer(const PropertyMap& props, const CameraConstPtr& camera,
                   const SceneConstPtr& scene, const SurfaceIntegratorPtr& surfaceIntegrator, JobScheduler& jobScheduler, StatsTracker& stats)
    : IRenderer(camera, scene), settings(props), surfaceIntegrator(surfaceIntegrator), jobScheduler(jobScheduler), stats(stats)
{
    RB_ASSERT(this->camera.get());
    RB_ASSERT(this->scene.get());
    RB_ASSERT(this->surfaceIntegrator.get());
}

void Renderer::Render()
{
    if (!camera || !scene || !surfaceIntegrator)
    {
        return;
    }

    srand(std::time(0));

    stats.Timer("Preprocessing", "Preprocessing time").Start();
    surfaceIntegrator->PreProcess(*scene, jobScheduler);
    stats.Timer("Preprocessing", "Preprocessing time").Stop();

    stats.Timer("Rendering", "Rendering time").Start();
    // Initialize rendering jobs
    // Each job covers a square work area with a side length of 16 pixels
    JobList jobs;
    int xStart=0, xEnd=0, yStart=0, yEnd=0;
    camera->GetPixelSampleExtents(xStart, yStart, xEnd, yEnd);
    for (int y = yStart; y <= yEnd; y += 16)
    {
        for (int x = xStart; x <= xEnd; x += 16)
        {
            RenderingWorkArea workArea = {x, std::min(xEnd, x+15), y, std::min(yEnd, y+15)};
            JobConstPtr job(new RenderingJob(rand(), workArea, camera.get(), scene.get(), surfaceIntegrator.get()));
            jobs.push_back(job);
        }
    }
    // Schedule and run the jobs
    jobScheduler.Spawn(jobs);
    jobScheduler.WaitForAllJobs();
    stats.Timer("Rendering", "Rendering time").Stop();
}
}
