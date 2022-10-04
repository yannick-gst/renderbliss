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

#ifndef RENDERBLISS_THIN_LENS_CAMERA_H
#define RENDERBLISS_THIN_LENS_CAMERA_H

#include "renderbliss/Interfaces/ICamera.h"
#include "renderbliss/Math/Geometry/Basis3.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Math/Sampling/Sampling.h"

namespace renderbliss
{
// A camera class based on the thin lens model
class ThinLensCamera : public ICamera
{
public:

    ThinLensCamera(const boost::shared_ptr<IFilm>& film,
                   uint32 pixelSamplerWidth,
                   const Vector3& eye,    // Position of the camera
                   const Vector3& lookat, // A point the camera is looking at
                   const Vector3& up,     // View up vector
                   float hfov,            // Horizontal field of view in degrees
                   float focusDistance,   // Distance to the view plane in meters
                   float aperture         // Camera aperture in meters
                   );

    ThinLensCamera(const boost::shared_ptr<IFilm>& film,
                   uint32 pixelSamplerWidth,
                   const Vector3& eye,    // Position of the camera
                   const Vector3& lookat, // A point in 3d space the camera is looking at
                   const Vector3& up,     // View up vector
                   float hfov,            // Horizontal field of view in meters
                   float focusDistance,   // Distance to the view plane in meters
                   float focalLength,     // Focal length in meters
                   float fstop            // Focal ratio (f-number)
                   );

    // Generates a ray to be traced
    void GenerateRay(const PixelSample& ps, Ray& ray) const;

private:

    Basis3 uvn;       // Viewing system
    Vector3 eye;      // Position of the camera
    Vector3 across;   // Upper side of the viewing plane, pointing to the right
    Vector3 down;     // Left side of the view plane, pointing down
    Vector3 corner;   // Upper left hand of the view plane
    float lensRadius; // Lens radius in meters

    // Initializes the corner and sides of the view plane
    void InitCornerAndSides(float hfov, float focusDistance);
};
}

#endif
