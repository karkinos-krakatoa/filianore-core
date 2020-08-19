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
#define FILIANORE_INLINE __attribute__((always_inline))
#endif

    // Forward Declrs..
    class Ray;
    struct AABB;
    class Transform;

    struct Interaction;
    class SurfaceInteraction;

    class Shape;
    class Primitive;
    class GeometricPrimitive;

    class Film;
    class Camera;
    class Sampler;

    class Spectrum;

    class BxDF;
    class BSDF;
    class Material;
    class Texture;

    class Medium;
    class MediumInteraction;
    struct MediumInterface;
    class BSSRDF;

    class Illuminant;
    class VisibilityEvaluator;
    class AreaIlluminant;
    struct Distribution1D;
    class Distribution2D;

    std::string AppInfo()
    {
        return "Filianore Foundation";
    }

} // namespace filianore

#endif