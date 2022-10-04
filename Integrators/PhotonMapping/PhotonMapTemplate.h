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

#ifndef RENDERBLISS_PHOTON_MAP_TEMPLATE_H
#define RENDERBLISS_PHOTON_MAP_TEMPLATE_H

#include <vector>
#include <boost/function.hpp>
#include "renderbliss/Types.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/BoundingBox.h"
#include "renderbliss/Integrators/PhotonMapping/Photon.h"

namespace renderbliss
{
struct Intersection;
class  PropertyMap;
struct Vector3;

// Building blocks of photon map data structure based on Henrik Wann Jensen's
// implementation in the book "Realistic Image Synthesis Using Photon Mapping".
// To be overriden for caustic and global photon storage.
template <typename PhotonType>
class PhotonMapTemplate
{
public:

    PhotonMapTemplate(const PropertyMap& props);

    // Creates a left-balanced kd-tree from the photon array. Must be called *once* after
    // all photons have been stored, *and* before the photon map is used for rendering.
    void Balance();

    // Returns whether the photon map is empty
    bool Empty() const;

    // Returns whether the target number of photons to store has been reached
    bool Full() const;

    // Returns the number of photons stored
    size_t PhotonCount() const;

    // Estimates the irradiance at a surface position and normal
    Spectrum IrradianceEstimate(const Vector3& point, const Vector3& normal, uint32 numLookup, real gatherRadius) const;

    // Estimates the radiance at a surface intersection towards a given direction
    virtual Spectrum RadianceEstimate(const Intersection& hit, const Vector3& outgoingDirection) = 0;

    // Scales photon power (must be called immediately before or immediately after balancing)
    void ScalePower(float scale);

protected:

    enum { MaxNearestPhotons = 5000 };
    enum { MaxStoredPhotons = 50000000 };

    // Used to locate nearest photons when computing a radiance estimate
    struct NearestPhoton
    {
        real sqrDist; // Squared distance from query location
        size_t index; // PhotonType index in the photon map
        NearestPhoton(real sqrDist = Infinity(), size_t index = 0) : sqrDist(sqrDist), index(index) {}
        bool operator<(const NearestPhoton& np) const { return sqrDist < np.sqrDist; }
    };

    // Finds 'numLookup' photons that are nearest to a given location, and within a given radius
    void LocatePhotons(const Vector3& queryLocation, uint32 numLookup, real& sqrMaxRadius, size_t nodeIndex, std::vector<NearestPhoton>& heap) const;

    struct Settings
    {
        real gatherRadius;
        uint32 numPhotonsToGather;
        uint32 numPhotonsToStore;
        Settings(const PropertyMap& props);
    } settings;

    std::vector<PhotonNode> nodes;
    std::vector<PhotonType> photons;

private:

    // Recursively balances a segment of the photon array:
    // - "start" points to the starting index of the segment
    // - "end" points one location past the end of the segment
    // - "nodeIndex" is the index of the kd-node being built
    void BalanceSegment(size_t nodeIndex, size_t start, size_t end, std::vector<PhotonType>& balancedPhotons);
};

// Returns a density estimate using a Simpson kernel function
real Kernel(real sqrDist, real sqrMaxDist);
}

#include "renderbliss/Integrators/PhotonMapping/PhotonMapTemplate.inl"

#endif
