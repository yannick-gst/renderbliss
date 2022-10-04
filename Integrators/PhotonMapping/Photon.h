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

#ifndef RENDERBLISS_PHOTON_H
#define RENDERBLISS_PHOTON_H

#include "renderbliss/Types.h"
#include "renderbliss/Colour/RGB.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Math/Geometry/Vector3.h"

namespace renderbliss
{
class Photon
{
public:

    // The constructor assumes that the direction is of unit length.
    Photon(const Vector3& position, const Vector3& direction, const Spectrum& power);
    Vector3 Direction() const;
    const Vector3& Position() const;
    Spectrum Power() const;
    void ScalePower(float scale);

private:

    Vector3 position;
    float dirPhi, dirTheta; // Direction in spherical coordinates
    RGBE power;
};

typedef Photon CausticPhoton;
typedef Photon DirectPhoton;

// Irradiance photons hold a precomputed irradiance estimate
class IrradiancePhoton : public Photon
{
public:
    // The constructor assumes that the direction and normal are of unit length.
    IrradiancePhoton(const Vector3& position, const Vector3& direction, const Vector3& normal, const Spectrum& power);
    Vector3 Normal() const;

private:

    float normalPhi, normalTheta; // Normal in spherical coordinates
};

// Helper data structure for cache-efficient photon lookup in the photon map
class PhotonNode
{
public:

    PhotonNode();
    uint32 FarChildIndex() const;
    bool HasFarChild() const;
    bool HasNearChild() const;
    bool IsLeaf() const;
    void SetFarChildIndex(uint32 index);
    void SetLeaf();
    void SetNearChildBit();
    void SetSplitInfo(real splitPosition, uint32 splitAxis);
    uint32 SplitAxis() const;
    real SplitPosition() const;

private:

    real splitPosition;
    uint32 splitAxis:2;
    uint32 hasNearChild:1;
    uint32 farChildIndex:29;
};
}

#endif
