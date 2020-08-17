#ifndef _ELEMENTAL_H
#define _ELEMENTAL_H

#include <string>

namespace filianore
{

#if defined(_MSC_VER)
#define FILIANORE_NOINLINE __declspec(noinline)
#define FILIANORE_INLINE __forceinline
#else
#define FILIANORE_NOINLINE __attribute__((noinline))
#define FILIANORE_INLINE __attribute__((always_inline)) inline
#endif

    // Forward Declrs..
    class Ray;
    class AABB;
    class Transform;

    class Film;
    class Camera;
    class Sampler;
    class PixelSampler;
    class GlobalSampler;

    std::string AppInfo()
    {
        return "Filianore Foundation";
    }

} // namespace filianore

#endif