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

#include "renderbliss/Integrators/PhotonMapping/Photon.h"
#include <cmath>
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Intersection.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Utils/PropertyMap.h"

namespace renderbliss
{
Photon::Photon(const Vector3& position, const Vector3& direction, const Spectrum& power)
    : position(position),
#ifdef SPECTRUM_IS_RGB
      power(power.ToRGBE())
#else
    power(power.ToRGB().ToRGBE())
#endif
{
    dirTheta = acosf(direction.z);
    dirPhi = atan2(direction.y, direction.x);
}

Vector3 Photon::Direction() const
{
    return SphericalToCartesian(dirPhi, dirTheta);
}

const Vector3& Photon::Position() const
{
    return position;
}

Spectrum Photon::Power() const
{
    return Spectrum(power);
}

void Photon::ScalePower(float scale)
{
    power = (RGB(power)*scale).ToRGBE();
}

IrradiancePhoton::IrradiancePhoton(const Vector3& position, const Vector3& direction, const Vector3& normal, const Spectrum& power)
    : Photon(position, direction, power)
{
    normalTheta = acosf(normal.z);
    normalPhi = atan2(normal.y, normal.x);
}

Vector3 IrradiancePhoton::Normal() const
{
    return SphericalToCartesian(normalPhi, normalTheta);
}

PhotonNode::PhotonNode()
{
    SetLeaf();
}

uint32 PhotonNode::FarChildIndex() const
{
    return farChildIndex;
}

bool PhotonNode::HasFarChild() const
{
    return farChildIndex < ((1 << 29) - 1);
}

bool PhotonNode::HasNearChild() const
{
    return hasNearChild == 1;
}

bool PhotonNode::IsLeaf() const
{
    return !HasNearChild() && !HasFarChild();
}

void PhotonNode::SetFarChildIndex(uint32 index)
{
    farChildIndex = index;
}

void PhotonNode::SetLeaf()
{
    hasNearChild = 0;
    farChildIndex = (1 << 29) - 1;
}

void PhotonNode::SetNearChildBit()
{
    hasNearChild = 1;
}

void PhotonNode::SetSplitInfo(real splitPosition, uint32 splitAxis)
{
    this->splitAxis = splitAxis;
    this->splitPosition = splitPosition;
}

uint32 PhotonNode::SplitAxis() const
{
    return splitAxis;
}

real PhotonNode::SplitPosition() const
{
    return splitPosition;
}
}
