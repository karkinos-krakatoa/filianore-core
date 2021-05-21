#ifndef _BINNED_SAH_BUILDER_H
#define _BINNED_SAH_BUILDER_H

#include "binnedsahbuildtask.h"

namespace filianore
{

    class BinnedSahBuilder : public TopDownBuilder
    {
        using TopDownBuilder::RunTask;
        Bvh &bvh;

    public:
        BinnedSahBuilder(Bvh &_bvh)
            : bvh(_bvh)
        {
        }

        void ExecuteBuild(
            const AABB &globalBounds,
            const AABB *bbounds,
            const StaticArray<float, 3> *centroids,
            size_t primitiveCount)
        {
            assert(primitiveCount > 0);

            bvh.nodes = std::make_unique<Bvh::BvhNode[]>(2 * primitiveCount + 1);
            bvh.primitiveIndices = std::make_unique<size_t[]>(primitiveCount);
            bvh.nodeCount = 1;
            bvh.nodes[0].AABB_Proxy() = globalBounds;

            //#pragma omp parallel
            {
                //#pragma omp for
                for (size_t i = 0; i < primitiveCount; ++i)
                {
                    bvh.primitiveIndices[i] = i;
                }

                //#pragma omp single
                {
                    BinnedSahBuildTask firstTask(bvh, bbounds, centroids);
                    WorkItem workItem(0, 0, primitiveCount, 0);
                    RunTask(firstTask, workItem);
                }
            }
        }
    };

}

#endif