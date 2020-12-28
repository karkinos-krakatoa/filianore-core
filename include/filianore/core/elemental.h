#ifndef _ELEMENTAL_H
#define _ELEMENTAL_H

#include <string>
#include <memory>

namespace filianore
{

#if defined(_MSC_VER)
#define FILIANORE_NOINLINE __declspec(noinline)
#define FILIANORE_INLINE __forceinline
#else
#define FILIANORE_NOINLINE __attribute__((noinline))
#define FILIANORE_INLINE __attribute__((always_inline))
#endif

    enum class TransportMode
    {
        Radiance,
        Importance
    };

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
    class PhysicalCamera;
    class Sampler;

    class Spectrum;

    class BxDF;
    class BRDF;
    class BTDF;
    class BSDF;
    class Material;
    template <typename T>
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

    class Scene;

    class MemoryArena;

} // namespace filianore

#endif