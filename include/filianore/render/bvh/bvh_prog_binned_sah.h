#ifndef _BVH_PROGRAM_BINNED_SAH_H
#define _BVH_PROGRAM_BINNED_SAH_H


#include "filianore/core/aabb.h"
#include "bvh.h"
#include "bvh_builder_topdown.h"
#include "bvh_alg_sah.h"


namespace filianore
{

    template<typename, size_t> class BvhProgramBinnedSahBuildTask;

    template <typename BVH, size_t BinCount>
    class BvhProgramBinnedSah : public BvhBuilderTopDown, public BvhAlgSah
    {
        using BuildTask = BvhProgramBinnedSahBuildTask<BVH, BinCount>;
        using BvhBuilderTopDown::RunTask;
        friend BuildTask;

        BVH& bvh;


    public:

        using BvhBuilderTopDown::maxDepth;
        using BvhBuilderTopDown::maxLeafSize;
        using BvhAlgSah::rayTraversalCost;

        BvhProgramBinnedSah(BVH& _bvh)
            : bvh(_bvh) {}


        void Build(
            const AABB& globalBox,
            const AABB* bboxes,
            const StaticArray<float, 3>* centers,
            size_t primitiveCount)
        {
            bvh.nodes = std::make_unique<typename BVH::Node[]>(2 * primitiveCount + 1);
            bvh.primitiveIndices = std::make_unique<size_t[]>(primitiveCount);
            bvh.nodeCount = 1;
            bvh.nodes[0].AABB_Proxy() = globalBox;

            #pragma omp parallel
            {
                #pragma omp for
                for (size_t i = 0; i < primitiveCount; ++i)
                {
                    bvh.primitiveIndices[i] = i;
                }

                #pragma omp single
                {
                    BuildTask firstTask(*this, bboxes, centers);
                    RunTask(firstTask, 0, 0, primitiveCount, 0);
                }
            }
        }

    };


    template <typename BVH, size_t BinCount>
    class BvhProgramBinnedSahTask : public BvhBuilderTopDownTask
    {
        using Builder = BvhProgramBinnedSah<BVH, BinCount>;
        using BvhBuilderTopDownTask::WorkItem;

        struct Bin
        {
            AABB bbox;
            size_t primitiveCount;
            float rightCost;
        };

        static constexpr size_t binCount = BinCount;
        std::array<Bin, BinCount> binsPerAxis[3];

        Builder& builder;
        const AABB* bboxes;
        const StaticArray<float, 3>* centers;

        std::pair<float, size_t> FindSplit(int axis)
        {
            auto& bins = binsPerAxis[axis];

            auto currentBBox = AABB::Empty();
            size_t currentCount = 0;

            for (size_t i = binCount - 1; i > 0; --i)
            {

            }

        }

    };

}


#endif