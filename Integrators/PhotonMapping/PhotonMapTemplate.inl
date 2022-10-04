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

#include <algorithm>
#include <functional>
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Utils/PropertyMap.h"

namespace renderbliss
{
inline real Kernel(real sqrDist, real sqrMaxDist)
{
    real s = (1.0f - sqrDist/sqrMaxDist);
    return 3.0f*InvPi()*s*s;
}

template <typename PhotonType>
PhotonMapTemplate<PhotonType>::Settings::Settings(const PropertyMap& props)
{
    props.Get<real>("gather_radius", 100.0f, gatherRadius);

    props.Get<uint32>("photons_to_gather", 100, numPhotonsToGather);
    if (numPhotonsToGather > PhotonMapTemplate<PhotonType>::MaxNearestPhotons)
    {
        numPhotonsToGather = PhotonMapTemplate<PhotonType>::MaxNearestPhotons;
    }

    props.Get<uint32>("photons_to_store", 0, numPhotonsToStore);
    if (numPhotonsToStore > PhotonMapTemplate<PhotonType>::MaxStoredPhotons)
    {
        numPhotonsToStore = PhotonMapTemplate<PhotonType>::MaxStoredPhotons;
    }
}

template <typename PhotonType>
PhotonMapTemplate<PhotonType>::PhotonMapTemplate(const PropertyMap& props)
    : settings(props)
{
}

template <typename PhotonType>
void PhotonMapTemplate<PhotonType>::Balance()
{
    nodes.clear();
    if (photons.empty()) return;
    std::vector<PhotonType> balancedPhotons;
    balancedPhotons.reserve(PhotonCount());
    BalanceSegment(0, 0, PhotonCount(), balancedPhotons);
    RB_ASSERT(photons.size() == nodes.size());
    RB_ASSERT(balancedPhotons.size() == nodes.size());
    photons.swap(balancedPhotons);
    std::vector<PhotonNode>(nodes).swap(nodes); // Shrink node storage capacity to fit contents
}

template <typename PhotonType>
void PhotonMapTemplate<PhotonType>::BalanceSegment(size_t nodeIndex, size_t start, size_t end, std::vector<PhotonType>& balancedPhotons)
{
    // Predicate functor for photon map balancing
    struct PhotonPositionComparer : public std::binary_function<PhotonType, PhotonType, bool>
    {
        uint32 axis;
        PhotonPositionComparer(uint32 axis) : axis(axis) {}
        bool operator()(const PhotonType& lhs, const PhotonType& rhs) const
        {
            return lhs.Position()[axis] < rhs.Position()[axis];
        }
    };

    RB_ASSERT(nodeIndex==nodes.size());
    nodes.push_back(PhotonNode());
    if (start+1 == end)
    {
        nodes[nodeIndex].SetLeaf();
        balancedPhotons.push_back(photons[start]);
        return;
    }

    size_t median = (end+start) / 2;
    std::vector<PhotonType>::iterator begIter = photons.begin() + start;
    std::vector<PhotonType>::iterator medIter = photons.begin() + median;
    std::vector<PhotonType>::iterator endIter = photons.begin() + end;

    // Partition photon block around the median
    BoundingBox segmentBound;
    for (size_t i = start; i < end; ++i)
    {
        segmentBound.Enclose(photons[i].Position());
    }
    Vector3 ext = segmentBound.Extents();
    uint32 splitAxis =   ((ext[0] > ext[1]) && (ext[0] > ext[2])) ? 0
                        : (ext[1] > ext[2]) ? 1
                        : 2;
    std::nth_element(begIter, medIter, endIter, PhotonPositionComparer(splitAxis));
    real splitPosition = photons[median].Position()[splitAxis];
    nodes[nodeIndex].SetSplitInfo(splitPosition, splitAxis);
    balancedPhotons.push_back(photons[median]);

    // Balance left segment
    if (start < median)
    {
        nodes[nodeIndex].SetNearChildBit();
        BalanceSegment(nodes.size(), start, median, balancedPhotons);
    }

    // Balance right segment
    if (median+1 < end)
    {
        nodes[nodeIndex].SetFarChildIndex(static_cast<uint32>(nodes.size()));
        BalanceSegment(nodes[nodeIndex].FarChildIndex(), median+1, end, balancedPhotons);
    }
}

template <typename PhotonType>
bool PhotonMapTemplate<PhotonType>::Empty() const
{
    return photons.empty();
}

template <typename PhotonType>
bool PhotonMapTemplate<PhotonType>::Full() const
{
    return PhotonCount() >= settings.numPhotonsToStore;
}

template <typename PhotonType>
Spectrum PhotonMapTemplate<PhotonType>::IrradianceEstimate(const Vector3& point, const Vector3& normal, uint32 numLookup, real gatherRadius) const
{
    if (Empty() || !numLookup)
    {
        return Spectrum::black;
    }

    std::vector<NearestPhoton> heap;
    heap.reserve(numLookup);
    real sqrMaxRadius = Sqr(gatherRadius);

    LocatePhotons(point, numLookup, sqrMaxRadius, 0, heap);

    if (heap.empty())
    {
        return Spectrum::black;
    }

    RB_ASSERT(heap.size() <= numLookup);
    if (heap.size() < numLookup-1)
    {
        std::make_heap(heap.begin(), heap.end());
        sqrMaxRadius = heap[0].sqrDist;
    }

    // Sum weighted irradiance from all nearest photons
    Spectrum irradiance;
    foreach (const NearestPhoton& np, heap)
    {
        const PhotonType& heapPhoton = photons[np.index];
        if (DotProduct(heapPhoton.Direction(), normal) > 0.0f)
        {
            real weight = Kernel(np.sqrDist, sqrMaxRadius);
            irradiance += weight*heapPhoton.Power();
        }
    }

    irradiance /= sqrMaxRadius;
    return irradiance;
}

template <typename PhotonType>
void PhotonMapTemplate<PhotonType>::LocatePhotons(const Vector3& queryLocation, uint32 numLookup, real& sqrMaxRadius, size_t nodeIndex, std::vector<NearestPhoton>& heap) const
{
    const PhotonNode& currentNode = nodes[nodeIndex];
    if (!currentNode.IsLeaf())
    {
        uint32 splitAxis = currentNode.SplitAxis();
        real splitPosition = currentNode.SplitPosition();
        real sqrRadius = Sqr(queryLocation[splitAxis] - splitPosition);
        if (queryLocation[splitAxis] <= splitPosition)
        {
            if (currentNode.HasNearChild())
            {
                LocatePhotons(queryLocation, numLookup, sqrMaxRadius, nodeIndex+1, heap);
            }
            if ((sqrRadius < sqrMaxRadius) && currentNode.HasFarChild())
            {
                LocatePhotons(queryLocation, numLookup, sqrMaxRadius, currentNode.FarChildIndex(), heap);
            }
        }
        else
        {
            if (currentNode.HasFarChild())
            {
                LocatePhotons(queryLocation, numLookup, sqrMaxRadius, currentNode.FarChildIndex(), heap);
            }
            if ((sqrRadius < sqrMaxRadius) && currentNode.HasNearChild())
            {
                LocatePhotons(queryLocation, numLookup, sqrMaxRadius, nodeIndex+1, heap);
            }
        }
    }

    real sqrDistToPhoton = SquaredDistance(photons[nodeIndex].Position(), queryLocation);
    if (sqrDistToPhoton < sqrMaxRadius)
    {
        heap.push_back(NearestPhoton(sqrDistToPhoton, nodeIndex));
        if (heap.size() < numLookup)
        {
            if (heap.size() == numLookup-1)
            {
                std::make_heap(heap.begin(), heap.end());
                sqrMaxRadius = heap[0].sqrDist;
            }
        }
        else
        {
            std::push_heap(heap.begin(), heap.end());
            if (heap.size() > numLookup)
            {
                // The heap is full, pop farthest photon
                std::pop_heap(heap.begin(), heap.end());
                heap.pop_back();
            }
            sqrMaxRadius = heap[0].sqrDist;
        }
    }
}

template <typename PhotonType>
size_t PhotonMapTemplate<PhotonType>::PhotonCount() const
{
    return photons.size();
}

template <typename PhotonType>
void PhotonMapTemplate<PhotonType>::ScalePower(float scale)
{
    foreach (PhotonType& p, photons) { p.ScalePower(scale); }
}
}
