#pragma once


#include <string>


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
    class Ray;
	class AABB;
	template <typename T> class Transform;

    class Film;
    class Camera;
    class Sampler;
    class PixelSampler;
    class GlobalSampler;
    
}