#ifndef _BVH_UTILS_H
#define _BVH_UTILS_H

#include <vector>

#include "../core/aabb.h"
#include "../core/util.h"
#include "../core/primitive.h"

namespace filianore
{

    std::pair<std::vector<std::shared_ptr<AABB>>,
              std::vector<std::shared_ptr<StaticArray<float, 3>>>>
    ComputeBoundingBoxesAndCenters(const std::vector<Primitive> primitives, size_t primitive_count)
    {
        std::vector<std::shared_ptr<AABB>> boundingBoxes;
        boundingBoxes.reserve(primitive_count);

        std::vector<std::shared_ptr<StaticArray<float, 3>>> centers;
        centers.reserve(primitive_count);

#pragma omp parallel for
        for (size_t i = 0; i < primitive_count; ++i)
        {
            boundingBoxes.emplace_back(primitives[i].WorldBound());
            centers.emplace_back(primitives[i].Centroid());
        }

        return std::make_pair(std::move(boundingBoxes), std::move(centers));
    }
} // namespace filianore

#endif