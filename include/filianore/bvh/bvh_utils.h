#ifndef _BVH_UTILS_H
#define _BVH_UTILS_H

#include <vector>
#include <utility>

#include "../core/aabb.h"
#include "../core/util.h"
#include "../core/primitive.h"

namespace filianore
{

    std::pair<std::vector<AABB>,
              std::vector<StaticArray<float, 3>>>
    ComputeBoundingBoxesAndCenters(const std::vector<std::shared_ptr<Primitive>> &primitives, size_t primitive_count)
    {
        std::vector<AABB> boundingBoxes;
        boundingBoxes.reserve(primitive_count);

        std::vector<StaticArray<float, 3>> centers;
        centers.reserve(primitive_count);

#pragma omp parallel for
        for (size_t i = 0; i < primitive_count; ++i)
        {
            boundingBoxes.emplace_back(primitives[i]->WorldBound());
            centers.emplace_back(primitives[i]->Centroid());
        }

        return std::make_pair(boundingBoxes, centers);
    }

    AABB ComputeEncompassingBoundingbox(const std::vector<AABB> &bboxes, size_t count)
    {
        auto bbox = AABB::Empty();

#pragma omp declare reduction(bbox_extend:AABB          \
                              : omp_out.Extend(omp_in)) \
    initializer(omp_priv = AABB::Empty())

#pragma omp parallel for reduction(bbox_extend \
                                   : bbox)
        for (size_t i = 0; i < count; ++i)
        {
            bbox.Extend(bboxes[i]);
        }

        return bbox;
    }

} // namespace filianore

#endif