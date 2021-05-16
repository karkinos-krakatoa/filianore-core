#ifndef _BVHX_UTILS_H
#define _BVHX_UTILS_H

#include <cmath>
#include <climits>
#include <type_traits>

namespace filianore
{
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
}

#endif