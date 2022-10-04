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

#include "renderbliss/Integrators/PhotonMapping/PhotonMap.h"
#include <algorithm>
#include <functional>
#include "renderbliss/Interfaces/IMaterial.h"
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Intersection.h"

namespace
{
    using namespace renderbliss;
}

namespace renderbliss
{
// Caustic/Direct photon map implementation

PhotonMap::PhotonMap(const PropertyMap& props) : PhotonMapTemplate<CausticPhoton>(props)
{
}

Spectrum PhotonMap::RadianceEstimate(const Intersection& hit, const Vector3& outgoingDirection)
{
    if (    Empty()
        || !settings.numPhotonsToGather
        || !hit.material
        || !hit.material->MatchesFlags(BsdfCombinedFlags::Diffuse | BsdfCombinedFlags::Glossy) /* Only evaluate non-specular BSDFs*/)
    {
        return Spectrum::black;
    }

    std::vector<NearestPhoton> heap;
    heap.reserve(settings.numPhotonsToGather);
    real sqrMaxRadius = Sqr(settings.gatherRadius);

    LocatePhotons(hit.point, settings.numPhotonsToGather, sqrMaxRadius, 0, heap);

    if (heap.empty())
    {
        return Spectrum::black;
    }

    RB_ASSERT(heap.size() <= settings.numPhotonsToGather);
    if (heap.size() < settings.numPhotonsToGather-1)
    {
        std::make_heap(heap.begin(), heap.end());
        sqrMaxRadius = heap[0].sqrDist;
    }

    // Get forward facing shading normal
    Vector3 shadingNormal = DotProduct(outgoingDirection, hit.uvn.N()) < 0.0f ? -hit.uvn.N() : hit.uvn.N();

    // Sum weighted irradiance from all nearest photons
    Spectrum radiance;
    foreach (const NearestPhoton& np, heap)
    {
        const Photon& heapPhoton = photons[np.index];

        // Depending on the orientation of the photon direction with respect
        // to the hit normal either reflection or refraction will be eliminated
        real dotDN = DotProduct(heapPhoton.Direction(), shadingNormal);
        int bsdfFlags = BsdfCombinedFlags::Diffuse | BsdfCombinedFlags::Glossy; // Only evaluate non-specular BSDFs
        bsdfFlags &= ((dotDN > 0.0f) ? ~BsdfCombinedFlags::Transmission : ~BsdfCombinedFlags::Reflection);

        // Evaluate the BSDF and accumulate the radiance estimate
        Spectrum bsdf = hit.material->BsdfCombinedFlagsValue(heapPhoton.Direction(), outgoingDirection, hit, bsdfFlags);
        if (!bsdf.IsBlack())
        {
            real weight = Kernel(np.sqrDist, sqrMaxRadius);
            radiance += weight * bsdf * heapPhoton.Power();
        }
    }

    radiance /= sqrMaxRadius;
    return radiance;
}

void PhotonMap::StorePhotons(const std::vector<Vector3>& positions, const std::vector<Vector3>& directions, const std::vector<Spectrum>& powers)
{
    if (!Full() && (positions.size() == directions.size()) && (directions.size() == powers.size()))
    {
        size_t count = positions.size();
        for (size_t i = 0; i < count; ++i)
        {
            if (PhotonCount() >= PhotonMapTemplate<CausticPhoton>::MaxStoredPhotons) break;
            photons.push_back(Photon(positions[i], directions[i], powers[i]));
        }
    }
}

// Global photon map implementation

IrradiancePhotonMap::IrradiancePhotonMap(const PropertyMap& props) : PhotonMapTemplate<IrradiancePhoton>(props)
{
    props.Get<uint32>("precomputed_irradiance_spacing", 4, spacing);
    props.Get<float>("precomputed_irradiance_normal_threshold", 0.9f, normalThreshold);
    Clamp(0.0f, 0.95f, normalThreshold);
}

bool IrradiancePhotonMap::LocateNearestPhoton(const Vector3& queryLocation, const Vector3& normal, real& sqrMaxRadius, size_t nodeIndex, NearestPhoton& nearest) const
{
    bool result = false;
    const PhotonNode& currentNode = nodes[nodeIndex];
    if (!currentNode.IsLeaf())
    {
        int splitAxis = currentNode.SplitAxis();
        real splitPosition = currentNode.SplitPosition();
        real sqrRadius = Sqr(queryLocation[splitAxis] - splitPosition);
        if (queryLocation[splitAxis] <= splitPosition)
        {
            if (currentNode.HasNearChild())
            {
                result |= LocateNearestPhoton(queryLocation, normal, sqrMaxRadius, nodeIndex+1, nearest);
            }
            if ((sqrRadius < sqrMaxRadius) && currentNode.HasFarChild())
            {
                result |= LocateNearestPhoton(queryLocation, normal, sqrMaxRadius, currentNode.FarChildIndex(), nearest);
            }
        }
        else
        {
            if (currentNode.HasFarChild())
            {
                result |= LocateNearestPhoton(queryLocation, normal, sqrMaxRadius, currentNode.FarChildIndex(), nearest);
            }
            if ((sqrRadius < sqrMaxRadius) && currentNode.HasNearChild())
            {
                result |= LocateNearestPhoton(queryLocation, normal, sqrMaxRadius, nodeIndex+1, nearest);
            }
        }
    }

    real sqrDistToPhoton = SquaredDistance(photons[nodeIndex].Position(), queryLocation);
    if ((sqrDistToPhoton < nearest.sqrDist) && (sqrDistToPhoton <= sqrMaxRadius))
    {
        if (   (DotProduct(photons[nodeIndex].Direction(), normal) > 0.0f)
            && (DotProduct(photons[nodeIndex].Normal(), normal) > normalThreshold))
        {
            nearest.index = nodeIndex;
            nearest.sqrDist = sqrMaxRadius = sqrDistToPhoton;
            result = true;
        }
    }

    return result;
}

void IrradiancePhotonMap::PrecomputeIrradianceEstimate(const CausticPhotonMap& causticPhotonMap, const DirectPhotonMap& directPhotonMap)
{
    std::vector<NearestPhoton> causticHeap, directHeap, indirectHeap;
    causticHeap.reserve(settings.numPhotonsToGather);
    directHeap.reserve(settings.numPhotonsToGather);
    indirectHeap.reserve(settings.numPhotonsToGather);

    size_t numStoredPhotons = PhotonCount();
    std::vector<IrradiancePhoton> irradiancePhotons;
    irradiancePhotons.reserve(numStoredPhotons/spacing);

    for (size_t i = 0; i < numStoredPhotons; i += spacing)
    {
        indirectHeap.clear();
        const IrradiancePhoton& ip = photons[i];

        Spectrum irradiance =   IrradianceEstimate(ip.Position(), ip.Normal(), settings.numPhotonsToGather, settings.gatherRadius)
                              + causticPhotonMap.IrradianceEstimate(ip.Position(), ip.Normal(), settings.numPhotonsToGather, settings.gatherRadius)
                              + directPhotonMap.IrradianceEstimate(ip.Position(), ip.Normal(), settings.numPhotonsToGather, settings.gatherRadius);

        irradiancePhotons.push_back(IrradiancePhoton(ip.Position(), ip.Direction(), ip.Normal(), irradiance));
    }

    photons.swap(irradiancePhotons);
    std::vector<IrradiancePhoton>().swap(irradiancePhotons);
    Balance();
}

Spectrum IrradiancePhotonMap::RadianceEstimate(const Intersection& hit, const Vector3& outgoingDirection)
{
    if (    Empty()
        || !settings.numPhotonsToGather
        || !hit.material
        || !hit.material->MatchesFlags(BsdfCombinedFlags::Diffuse | BsdfCombinedFlags::Glossy) /* Only evaluate non-specular BSDFs*/)
    {
        return Spectrum::black;
    }

    NearestPhoton nearest;
    real sqrMaxRadius = settings.gatherRadius*settings.gatherRadius;
    if (!LocateNearestPhoton(hit.point, hit.uvn.N(), sqrMaxRadius, 0, nearest))
    {
        return Spectrum::black;
    }

    // Get forward facing shading normal
    Vector3 shadingNormal = DotProduct(outgoingDirection, hit.uvn.N()) < 0.0f ? -hit.uvn.N() : hit.uvn.N();

    // Depending on the orientation of the photon direction with respect
    // to the hit normal either reflection or refraction will be eliminated
    real dotDN = DotProduct(photons[nearest.index].Direction(), shadingNormal);
    int bsdfFlags = BsdfCombinedFlags::Diffuse | BsdfCombinedFlags::Glossy; // Only evaluate non-specular BSDFs
    bsdfFlags &= ((dotDN > 0.0f) ? ~BsdfCombinedFlags::Transmission : ~BsdfCombinedFlags::Reflection);

    // Evaluate the BSDF and accumulate the radiance estimate
    Spectrum bsdf = hit.material->BsdfCombinedFlagsValue(shadingNormal, outgoingDirection, hit, bsdfFlags);

    return photons[nearest.index].Power()*bsdf;
}

void IrradiancePhotonMap::StorePhotons(const std::vector<Vector3>& positions, const std::vector<Vector3>& directions, const std::vector<Vector3>& normals, const std::vector<Spectrum>& powers)
{
    if (!Full() && (positions.size() == directions.size()) && (directions.size() == powers.size()))
    {
        size_t count = positions.size();
        for (size_t i = 0; i < count; ++i)
        {
            if (PhotonCount() >= PhotonMapTemplate<IrradiancePhoton>::MaxStoredPhotons) break;
            photons.push_back(IrradiancePhoton(positions[i], directions[i], normals[i], powers[i]));
        }
    }
}
}
