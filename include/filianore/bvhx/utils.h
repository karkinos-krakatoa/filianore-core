#ifndef _BVHX_UTILS_H
#define _BVHX_UTILS_H

#include <cstring>
#include <cstdint>
#include <atomic>
#include <memory>
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>
#include <type_traits>
#include "../core/aabb.h"
#include "../core/primitive.h"

namespace filianore
{
    static constexpr size_t maxDepth = 64;
    static constexpr size_t maxLeafSize = 16;
    static constexpr size_t taskSpawnThreshold = 1024;
    static constexpr float traversalCost = 1.f;

    template <size_t Bits>
    struct SizedIntegerType
    {
        static_assert(Bits <= 8);
        using Signed = int8_t;
        using Unsigned = uint8_t;
    };
    template <>
    struct SizedIntegerType<64>
    {
        using Signed = int64_t;
        using Unsigned = uint64_t;
    };
    template <>
    struct SizedIntegerType<32>
    {
        using Signed = int32_t;
        using Unsigned = uint32_t;
    };
    template <>
    struct SizedIntegerType<16>
    {
        using Signed = int16_t;
        using Unsigned = uint16_t;
    };

    template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
    float AddUlpMagnitude(T x, unsigned ulps)
    {
        using U = typename SizedIntegerType<sizeof(T) * CHAR_BIT>::Unsigned;
        return std::isfinite(x) ? as<T>(as<U>(x) + ulps) : x;
    }

    std::pair<std::unique_ptr<AABB[]>, std::unique_ptr<StaticArray<float, 3>[]>> ComputeBoundingBoxesAndCenters(const std::vector<std::shared_ptr<Primitive>> &primitives, size_t primitiveCount)
    {
        std::unique_ptr<AABB[]> bbounds = std::make_unique<AABB[]>(primitiveCount);
        std::unique_ptr<StaticArray<float, 3>[]> centroids = std::make_unique<StaticArray<float, 3>[]>(primitiveCount);

#pragma omp parallel for
        for (size_t i = 0; i < primitiveCount; ++i)
        {
            bbounds[i] = primitives[i]->WorldBound();
            centroids[i] = primitives[i]->Centroid();
        }

        return std::make_pair(std::move(bbounds), std::move(centroids));
    }

    AABB ComputeGlobalBounds(const AABB *bbounds, size_t count)
    {
        AABB bbox = AABB::Empty();
        for (size_t i = 0; i < count; ++i)
        {
            bbox.Extend(bbounds[i]);
        }

        return bbox;
    }
}

#endif