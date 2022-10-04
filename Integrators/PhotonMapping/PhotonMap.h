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

#ifndef RENDERBLISS_PHOTON_MAP_H
#define RENDERBLISS_PHOTON_MAP_H

#include <vector>
#include <boost/function.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Integrators/PhotonMapping/PhotonMapTemplate.h"

namespace renderbliss
{
class PhotonMap : public PhotonMapTemplate<CausticPhoton>
{
public:

    PhotonMap(const PropertyMap& props);
    // Estimates the radiance at a surface intersection towrads a given direction
    Spectrum RadianceEstimate(const Intersection& hit, const Vector3& outgoingDirection);
    // Creates and stores photons
    void StorePhotons(const std::vector<Vector3>& positions, const std::vector<Vector3>& directions, const std::vector<Spectrum>& powers);
};

typedef PhotonMap CausticPhotonMap;
typedef PhotonMap DirectPhotonMap;

class IrradiancePhotonMap : public PhotonMapTemplate<IrradiancePhoton>
{
public:

    IrradiancePhotonMap(const PropertyMap& props);
    // Precomputes the irradiance estimate at every 'spacing'-th photon, given a caustic and a direct photon map for the same scene
    void PrecomputeIrradianceEstimate(const CausticPhotonMap& causticPhotonMap, const DirectPhotonMap& directPhotonMap);
    // Estimates the radiance at a surface intersection towards a given direction
    Spectrum RadianceEstimate(const Intersection& hit, const Vector3& outgoingDirection);
    // Creates and stores photons
    void StorePhotons(const std::vector<Vector3>& positions, const std::vector<Vector3>& directions, const std::vector<Vector3>& normals, const std::vector<Spectrum>& powers);

private:

    uint32 spacing;
    float normalThreshold; // Normal threshold for irradiance photon lookup
    // Finds the photon nearest to the hit location within a search radius
    bool LocateNearestPhoton(const Vector3& queryLocation, const Vector3& normal, real& sqrMaxRadius, size_t nodeIndex, NearestPhoton& nearest) const;
};
}

#endif
