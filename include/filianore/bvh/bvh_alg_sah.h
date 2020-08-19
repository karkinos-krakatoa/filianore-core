#ifndef _BVH_ALG_SAH_H
#define _BVH_ALG_SAH_H

#include "bvh.h"

namespace filianore
{

    class SahBasedAlgorithm
    {
    public:
        float rayTraversalCost = 1.f;

    protected:
        ~SahBasedAlgorithm() {}

        float ComputeCost(const Bvh &bvh) const
        {
            float cost(0.f);

#pragma omp parallel for reduction(+ \
                                   : cost)
            for (size_t i = 0; i < bvh.nodeCount; ++i)
            {
                if (bvh.nodes[i]->isLeaf)
                {
                    cost += bvh.nodes[i]->AABB_Proxy().HalfArea() * bvh.nodes[i]->primitiveCount;
                }
                else
                {
                    cost += rayTraversalCost * bvh.nodes[i]->AABB_Proxy().HalfArea();
                }
            }

            return cost / bvh.nodes[0]->AABB_Proxy().HalfArea();
        }
    };

} // namespace filianore

#endif