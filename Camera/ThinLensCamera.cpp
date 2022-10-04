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

#include "renderbliss/Camera/ThinLensCamera.h"
#include "renderbliss/Macros.h"
#include "renderbliss/Interfaces/IFilm.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Ray.h"
#include "renderbliss/Math/Geometry/Vector2.h"

namespace
{
using namespace renderbliss;
void ConcentricSampleDisk(real u1, real u2, Vector2& diskSample)
{
    // Original code by Peter Shirley and Kenneth Chiu
    // See: "A Low Distortion Map Between Disk And Square"; Journal of Graphics Tools, Vol. 2, No. 3; http://jgt.akpeters.com/papers/ShirleyChiu97/
    /*
    This transforms points on [0,1]^2 to points on unit disk centered at
    origin.  Each "pie-slice" quadrant of square is handled as a seperate
    case.  The bad floating point cases are all handled appropriately.
    The regions for (a,b) are:

                phi = pi/2
                -----*-----
                |\       /|
                |  \ 2 /  |
                |   \ /   |
         phi=pi * 3  *  1 * phi = 0
                |   / \   |
                |  / 4 \  |
                |/       \|
                -----*-----
                phi = 3pi/2
    */

    real a = 2.0f*u1 - 1;
    real b = 2.0f*u2 - 1;
    // (a,b) is now on [-1,1]^2
    real phi, r;

    if (a > -b) // Region 1 or 2
    {
        if (a > b) // Region 1, also |a| > |b|
        {
            r = a;
            phi = (FourthPi()) * (b/a);
        }
        else // Region 2, also |b| > |a|
        {
            r = b;
            phi = (FourthPi()) * (2 - (a/b));
        }
    }
    else // Region 3 or 4
    {
        if (a < b) // Region 3, also |a| >= |b|, a != 0
        {
            r = -a;
            phi = (FourthPi()) * (4 + (b/a));
        }
        else // Region 4, |b| >= |a|, but a==0 and b==0 could occur.
        {
            r = -b;
            phi = (b != 0.0f) ? (FourthPi()) * (6 - (a/b)) : 0.0f;
        }
    }

    diskSample.x = r*cos(phi);
    diskSample.y = r*sin(phi);
}
}

namespace renderbliss
{
ThinLensCamera::ThinLensCamera(const boost::shared_ptr<IFilm>& film, uint32 pixelSamplerWidth,
                               const Vector3& eye, const Vector3& lookat, const Vector3& up,
                               float hfov, float focusDistance, float aperture)
    : ICamera(film, pixelSamplerWidth), uvn(Basis3::CreateFromVN(up, eye-lookat)), eye(eye), lensRadius(0.5f*fabs(aperture))
{
    RB_ASSERT(film);
    InitCornerAndSides(hfov, focusDistance);
}

ThinLensCamera::ThinLensCamera(const boost::shared_ptr<IFilm>& film, uint32 pixelSamplerWidth,
                               const Vector3& eye, const Vector3& lookat, const Vector3& up,
                               float hfov, float focusDistance, float focalLength, float fstop)
    : ICamera(film, pixelSamplerWidth), uvn(Basis3::CreateFromVN(up, eye-lookat)), eye(eye), lensRadius(0.5f*fabs(focalLength/fstop))
{
    RB_ASSERT(film);
    InitCornerAndSides(hfov, focusDistance);
}

void ThinLensCamera::GenerateRay(const PixelSample& ps, Ray& ray) const
{
    Vector2 lensPoint;
    ConcentricSampleDisk(ps.lensSample[0], ps.lensSample[1], lensPoint);

    Vector3 origin = eye + lensRadius*(lensPoint[0]*uvn.U() + lensPoint[1]*uvn.V());
    Vector3 target = corner + ps.imageSample[0]*film->Dx()*across + ps.imageSample[1]*film->Dy()*down;

    ray.Move(origin);
    ray.Redirect(target-origin);
    ray.tmin = Epsilon();
    ray.tmax = Infinity();
}

void ThinLensCamera::InitCornerAndSides(float hfov, float focusDistance)
{
    hfov = DegToRad(hfov);
    Clamp(static_cast<real>(0.0f), HalfPi(), hfov);

    float aspectRatio = film->XResolution() * film->Dy();
    float w = 2.0f*focusDistance*tan(0.5f*hfov);
    float h = w/aspectRatio;

    across = w*uvn.U();
    down = -h*uvn.V();
    corner = eye - 0.5f*across - 0.5f*down - focusDistance*uvn.N();
}
}
