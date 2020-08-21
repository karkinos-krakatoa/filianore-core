#ifndef _BVH_UTILS_H
#define _BVH_UTILS_H

#include <vector>
#include <utility>

#include "../core/aabb.h"
#include "../core/util.h"
#include "../core/primitive.h"

namespace filianore
{

    const float &RobustMin(const float &x, const float &y)
    {
        return x < y ? x : y;
    }

    const float &RobustMax(const float &x, const float &y)
    {
        return x > y ? x : y;
    }

    template <typename To, typename From>
    To as(From from)
    {
        static_assert(sizeof(To) == sizeof(From));
        To to;
        std::memcpy(&to, &from, sizeof(from));
        return to;
    }

    float AddUlpMagnitude(float x, unsigned ulps)
    {
        using U = typename SizedIntegerType<sizeof(float) * CHAR_BIT>::Unsigned;
        return std::isfinite(x) ? as<float>(as<U>(x) + ulps) : x;
    }

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