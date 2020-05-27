#pragma once

#include <string>

using namespace std;

namespace filianore
{

#if defined(_MSC_VER)
# define FILIANORE_NOINLINE          __declspec(noinline)
# define FILIANORE_INLINE            __forceinline
#else
# define FILIANORE_NOINLINE          __attribute__((noinline))
# define FILIANORE_INLINE            __attribute__((always_inline)) inline
#endif


    // Forward Declrs..
    template <typename T, size_t N> class Ray;
	template <typename T> class AABB;
	template <typename T> class Transform;

}