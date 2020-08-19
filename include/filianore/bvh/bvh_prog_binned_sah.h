#ifndef _BVH_PROGRAM_BINNED_SAH_H
#define _BVH_PROGRAM_BINNED_SAH_H

#include <optional>

#include "../core/aabb.h"
#include "bvh.h"
#include "bvh_builder_topdown.h"
#include "bvh_alg_sah.h"

namespace filianore
{

    template <typename, size_t>
    class BinnedSahBuildTask;

    template <typename Bvh, size_t BinCount>
    class BinnedSahBuilder : public TopDownBuilder, public SahBasedAlgorithm
    {
        using BuildTask = BinnedSahBuildTask<Bvh, BinCount>;

        using TopDownBuilder::RunTask;

        friend BuildTask;

        Bvh &bvh;

    public:
        using SahBasedAlgorithm::rayTraversalCost;
        using TopDownBuilder::maxDepth;
        using TopDownBuilder::maxLeafSize;

        BinnedSahBuilder(Bvh &bvh)
            : bvh(bvh)
        {
        }

        void Build(
            const AABB &global_bbox,
            const AABB *bboxes,
            const StaticArray<float, 3> *centers,
            size_t primitive_count)
        {
            // Allocate buffers
            bvh.nodes.reserve(2 * primitive_count + 1);
            bvh.nodes.resize(2 * primitive_count + 1);

            bvh.primitiveIndices.reserve(primitive_count);
            bvh.primitiveIndices.resize(primitive_count);

            bvh.nodeCount = 1;
            bvh.nodes[0].AABB_Proxy() = global_bbox;

#pragma omp parallel
            {
#pragma omp for
                for (size_t i = 0; i < primitive_count; ++i)
                    bvh.primitiveIndices[i] = i;

#pragma omp single
                {
                    BuildTask firstTask(*this, bboxes, centers);
                    RunTask(firstTask, 0, 0, primitive_count, 0);
                }
            }
        }
    };

    template <typename Bvh, size_t BinCount>
    class BinnedSahBuildTask : public TopDownBuildTask
    {
        using Builder = BinnedSahBuilder<Bvh, BinCount>;

        using TopDownBuildTask::WorkItem;

        struct Bin
        {
            AABB bbox;
            size_t primitiveCount;
            float rightCost;
        };

        static constexpr size_t binCount = BinCount;
        std::array<Bin, binCount> binsPerAxis[3];

        Builder &builder;
        const AABB *bboxes;
        const StaticArray<float, 3> *centers;

        std::pair<float, size_t> FindSplit(int axis)
        {
            auto &bins = binsPerAxis[axis];

            // Right sweep to compute partial SAH
            auto currentBbox = AABB::Empty();
            size_t currCount = 0;
            for (size_t i = binCount - 1; i > 0; --i)
            {
                currentBbox.Extend(bins[i].bbox);
                currCount += bins[i].primitiveCount;
                bins[i].rightCost = currentBbox.HalfArea() * currCount;
            }

            // Left sweep to compute full cost and find minimum
            currentBbox = AABB::Empty();
            currCount = 0;

            auto bestSplit = std::pair<float, size_t>(std::numeric_limits<float>::max(), binCount);
            for (size_t i = 0; i < binCount - 1; ++i)
            {
                currentBbox.Extend(bins[i].bbox);
                currCount += bins[i].primitiveCount;
                auto cost = currentBbox.HalfArea() * currCount + bins[i + 1].rightCost;
                if (cost < bestSplit.first)
                    bestSplit = std::make_pair(cost, i + 1);
            }
            return bestSplit;
        }

    public:
        using WorkItemType = WorkItem;

        BinnedSahBuildTask(Builder &builder, const AABB *bboxes, const StaticArray<float, 3> *centers)
            : builder(builder), bboxes(bboxes), centers(centers)
        {
        }

        void Build(const WorkItem &item)
        {
            auto &bvh = builder.bvh;
            auto &node = bvh.nodes[item.nodeIndex];

            auto makeLeaf = [](typename Bvh::Node &node, size_t begin, size_t end) {
                node.firstChildOrPrimitive = begin;
                node.primitiveCount = end - begin;
                node.isLeaf = true;
            };

            if (item.WorkSize() <= 1 || item.depth >= builder.maxDepth)
            {
                makeLeaf(node, item.begin, item.end);
                //return std::nullopt;
            }

            auto primitiveIndices = bvh.primitiveIndices.get();

            std::pair<float, size_t> bestSplits[3];

            AABB bbox = node.AABB_Proxy().ToBoundingBox();
            StaticArray<float, 3> centerToBin = bbox.Diagonal().Inverse() * float(binCount);
            StaticArray<float, 3> binOffset = bbox.pMin.Neg() * centerToBin;
            auto computeBinIndex = [=](const StaticArray<float, 3> &center, int axis) {
                auto binIndex = center.params[axis] * centerToBin.params[axis] + binOffset.params[axis];
                return std::min(binCount - 1, size_t(std::max(0.f, binIndex)));
            };

            // Setup bins
            for (int axis = 0; axis < 3; ++axis)
            {
                for (auto &bin : binsPerAxis[axis])
                {
                    bin.bbox = AABB::Empty();
                    bin.primitiveCount = 0;
                }
            }

            // Fill bins with primitives
            for (size_t i = item.begin; i < item.end; ++i)
            {
                auto primitiveIndex = bvh.primitiveIndices[i];
                for (int axis = 0; axis < 3; ++axis)
                {
                    Bin &bin = binsPerAxis[axis][computeBinIndex(centers[primitiveIndex], axis)];
                    bin.primitiveCount++;
                    bin.bbox.Extend(bboxes[primitiveIndex]);
                }
            }

            for (int axis = 0; axis < 3; ++axis)
            {
                bestSplits[axis] = FindSplit(axis);
            }

            int bestAxis = 0;
            if (bestSplits[0].first > bestSplits[1].first)
                bestAxis = 1;
            if (bestSplits[bestAxis].first > bestSplits[2].first)
                bestAxis = 2;

            auto splitIndex = bestSplits[bestAxis].second;

            // Make sure the cost of splitting does not exceed the cost of not splitting
            auto maxSplitCost = node.AABB_Proxy().HalfArea() * (item.WorkSize() - builder.rayTraversalCost);
            if (bestSplits[bestAxis].second == binCount || bestSplits[bestAxis].first >= maxSplitCost)
            {
                if (item.WorkSize() > builder.maxLeafSize)
                {
                    // Fallback strategy: approximate median split on largest axis
                    bestAxis = node.AABB_Proxy().ToBoundingBox().LargestAxis();
                    for (size_t i = 0, count = 0; i < binCount - 1; ++i)
                    {
                        count += binsPerAxis[bestAxis][i].primitiveCount;
                        // Split when we reach 0.4 times the number of primitives in the node
                        if (count >= (item.WorkSize() * 2 / 5 + 1))
                        {
                            splitIndex = i + 1;
                            break;
                        }
                    }
                }
                else
                {
                    makeleaf(node, item.begin, item.end);
                    //return std::nullopt;
                }
            }

            // Split primitives according to split position
            size_t beginRight = std::partition(primitiveIndices + item.begin, primitiveIndices + item.end, [&](size_t i) {
                                    return computeBinIndex(centers[i], bestAxis) < splitIndex;
                                }) -
                                primitiveIndices;

            // Check that the split does not leave one side empty
            if (beginRight > item.begin && beginRight < item.end)
            {
                // Allocate two nodes
                size_t firstChild;
#pragma omp atomic capture
                {
                    firstChild = bvh.nodeCount;
                    bvh.nodeCount += 2;
                }

                auto &left = bvh.nodes[firstChild + 0];
                auto &right = bvh.nodes[firstChild + 1];
                node.firstChildOrPrimitive = firstChild;
                node.primitiveCount = 0;
                node.isLeaf = false;

                // Compute the bounding boxes of each node
                auto &bins = binsPerAxis[bestAxis];
                auto leftBbox = AABB::Empty();
                auto rightBbox = AABB::Empty();
                for (size_t i = 0; i < bestSplits[bestAxis].second; ++i)
                    leftBbox.Extend(bins[i].bbox);
                for (size_t i = splitIndex; i < binCount; ++i)
                    rightBbox.Extend(bins[i].bbox);
                left.AABB_Proxy() = leftBbox;
                right.AABB_Proxy() = rightBbox;

                // Return new work items
                WorkItem firstItem(firstChild + 0, item.begin, beginRight, item.depth + 1);
                WorkItem secondItem(firstChild + 1, beginRight, item.end, item.depth + 1);
                //return std::make_optional(std::make_pair(firstItem, secondItem));
            }

            makeLeaf(node, item.begin, item.end);
            //return std::nullopt;
        }
    };

} // namespace filianore

#endif