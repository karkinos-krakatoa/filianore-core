#ifndef _BINNED_SAH_BUILD_TASK_H
#define _BINNED_SAH_BUILD_TASK_H

#include "bvhx.h"
#include "../core/aabb.h"
#include "topdownbuilder.h"

namespace filianore
{

    class BinnedSahBuildTask : public TopDownBuildTask
    {
        using TopDownBuildTask::WorkItem;
        using BvhNode = Bvh::BvhNode;

        struct Bin
        {
            AABB bbox;
            size_t primitiveCount;
            float rightCost;
        };

        static constexpr size_t binCount = 8;
        std::array<Bin, binCount> binsPerAxis[3];

        Bvh &bvh;
        const AABB *bbounds;
        const StaticArray<float, 3> *centroids;

        std::pair<float, size_t> FindSplit(int axis)
        {
            std::array<Bin, binCount> &bins = binsPerAxis[axis];

            // Right Sweep
            AABB currentBox = AABB::Empty();
            size_t currentCount = 0;
            for (size_t i = binCount - 1; i > 0; --i)
            {
                currentBox.Extend(bins[i].bbox);
                currentCount += bins[i].primitiveCount;
                bins[i].rightCost = currentBox.HalfArea() * currentCount;
            }

            // Left Sweep
            currentBox = AABB::Empty();
            currentCount = 0;

            std::pair<float, size_t> bestSplit = std::pair<float, size_t>(std::numeric_limits<float>::max(), binCount);
            for (size_t i = 0; i < binCount - 1; ++i)
            {
                currentBox.Extend(bins[i].bbox);
                currentCount += bins[i].primitiveCount;
                float cost = currentBox.HalfArea() * currentCount + bins[i + 1].rightCost;
                if (cost < bestSplit.first)
                {
                    bestSplit = std::make_pair(cost, i + 1);
                }
            }

            return bestSplit;
        }

    public:
        BinnedSahBuildTask(Bvh &_bvh, const AABB *_bbounds, const StaticArray<float, 3> *_centroids)
            : bvh(_bvh), bbounds(_bbounds), centroids(_centroids)
        {
        }

        std::optional<std::pair<WorkItem, WorkItem>> Build(const WorkItem &workItem)
        {
            BvhNode node = bvh.nodes[workItem.nodeIndex];

            auto MakeLeaf = [](BvhNode &node, size_t begin, size_t end)
            {
                node.firstChildOrPrimitive = begin;
                node.primitiveCount = end - begin;
            };

            if (workItem.WorkSize() <= 1 || workItem.depth >= maxDepth)
            {
                // No more work, create a leaf
                MakeLeaf(node, workItem.begin, workItem.end);
                return std::nullopt;
            }

            size_t *primitiveIndices = bvh.primitiveIndices.get();

            std::pair<float, size_t> bestSplits[3];

            AABB bbox = node.AABB_Proxy().ToBoundingBox();
            StaticArray<float, 3> centerToBin = bbox.Diagonal().Inverse() * (float)binCount;
            StaticArray<float, 3> binOffset = bbox.pMin.Neg() * centerToBin;

            auto ComputeBinIndex = [=](const StaticArray<float, 3> &centroid, int axis)
            {
                float binIndex = std::fmaf(centroid.params[axis], centerToBin.params[axis], binOffset.params[axis]);
                return std::min(binCount - 1, size_t(std::max(0.f, binIndex)));
            };

            // Setup Bins
            for (int axis = 0; axis < 3; ++axis)
            {
                for (auto &bin : binsPerAxis[axis])
                {
                    bin.bbox = AABB::Empty();
                    bin.primitiveCount = 0;
                }
            }

            // Fill Bins with Prims
            for (size_t i = workItem.begin; i < workItem.end; ++i)
            {
                size_t primitiveIndex = bvh.primitiveIndices[i];
                for (int axis = 0; axis < 3; ++axis)
                {
                    auto binIndex = ComputeBinIndex(centroids[primitiveIndex], axis);
                    Bin &bin = binsPerAxis[axis][binIndex];
                    bin.primitiveCount++;
                    bin.bbox.Extend(bbounds[primitiveIndex]);
                }
            }

            for (int axis = 0; axis < 3; ++axis)
            {
                bestSplits[axis] = FindSplit(axis);
            }

            int bestAxis = 0;
            if (bestSplits[0].first > bestSplits[1].first)
            {
                bestAxis = 1;
            }
            if (bestSplits[bestAxis].first > bestSplits[2].first)
            {
                bestAxis = 2;
            }

            size_t splitIndex = bestSplits[bestAxis].second;

            // Is the SAH Split even worth it ?
            float maxSplitCost = node.AABB_Proxy().HalfArea() * (workItem.WorkSize() - traversalCost);
            if (bestSplits[bestAxis].second == binCount || bestSplits[bestAxis].first >= maxSplitCost)
            {
                if (workItem.WorkSize() > maxLeafSize)
                {
                    // Last Resort : Median Split Strategy
                    bestAxis = node.AABB_Proxy().ToBoundingBox().LargestAxis();
                    for (size_t i = 0, count = 0; i < binCount - 1; ++i)
                    {
                        count += binsPerAxis[bestAxis][i].primitiveCount;

                        if (count >= (workItem.WorkSize() * 2 / 5 + 1))
                        {
                            splitIndex = i + 1;
                            break;
                        }
                    }
                }
                else
                {
                    MakeLeaf(node, workItem.begin, workItem.end);
                    return std::nullopt;
                }
            }

            // SAH Splitting
            size_t beginRight = std::partition(primitiveIndices + workItem.begin, primitiveIndices + workItem.end,
                                               [&](size_t i)
                                               {
                                                   return ComputeBinIndex(centroids[i], bestAxis) < splitIndex;
                                               }) -
                                primitiveIndices;

            if (beginRight > workItem.begin && beginRight < workItem.end)
            {
                size_t firstChild;

                //#pragma omp atomic capture
                {
                    firstChild = bvh.nodeCount;
                    bvh.nodeCount += 2;
                }

                BvhNode &left = bvh.nodes[firstChild + 0];
                BvhNode &right = bvh.nodes[firstChild + 2];

                node.firstChildOrPrimitive = firstChild;
                node.primitiveCount = 0;

                std::array<Bin, binCount> &bins = binsPerAxis[bestAxis];
                AABB leftBbox = AABB::Empty();
                AABB rightBbox = AABB::Empty();

                for (size_t i = 0; i < bestSplits[bestAxis].second; ++i)
                {
                    leftBbox.Extend(bins[i].bbox);
                }

                for (size_t i = splitIndex; i < binCount; ++i)
                {
                    rightBbox.Extend(bins[i].bbox);
                }

                left.AABB_Proxy() = leftBbox;
                right.AABB_Proxy() = rightBbox;

                // New Work Items
                WorkItem firstWorkItem(firstChild + 0, workItem.begin, beginRight, workItem.depth + 1);
                WorkItem secondWorkItem(firstChild + 2, beginRight, workItem.end, workItem.depth + 1);

                return std::make_pair(firstWorkItem, secondWorkItem);
            }

            MakeLeaf(node, workItem.begin, workItem.end);
            return std::nullopt;
        }
    };

}

#endif