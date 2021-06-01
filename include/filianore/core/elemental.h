#ifndef _ELEMENTAL_H
#define _ELEMENTAL_H

#include <stdio.h>
#include <string>
#include <memory>
#include <cstring>
#ifdef FILIANORE_HAVE_MALLOC_H
#include <malloc.h> // for _alloca, memalign
#endif
#ifdef FILIANORE_HAVE_ALLOCA_H
#include <alloca.h>
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

#if defined(_MSC_VER)
#define FILIANORE_NOINLINE __declspec(noinline)
#define FILIANORE_INLINE __forceinline
#else
#define FILIANORE_NOINLINE __attribute__((noinline))
#define FILIANORE_INLINE __attribute__((always_inline))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define Likely(x) __builtin_expect(x, true)
#define Unlikely(x) __builtin_expect(x, false)
#else
#define Likely(x) x
#define Unlikely(x) x
#endif

#if defined(_WIN32) || defined(_WIN64)
#define FILIANORE_IS_WINDOWS
#endif

#ifndef FILIANORE_L1_CACHE_LINE_SIZE
#define FILIANORE_L1_CACHE_LINE_SIZE 64
#endif

#define ALLOCA(TYPE, COUNT) (TYPE *)alloca((COUNT) * sizeof(TYPE))

namespace filianore
{

    enum class TransportMode
    {
        Radiance,
        Importance
    };

    // Forward Declrs..
    class Ray;
    template <typename T>
    class Rect;
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

    class Fresnel;
    class FresnelDielectric;

    class BxDF;
    class BRDF;
    class BTDF;
    class BSDF;
    class Material;
    class MicrofacetDistribution;
    template <typename T>
    class Texture;

    enum class SpectrumType
    {
        REFLECTANCE,
        ILLUMINANT
    };

    template <int nSpectrumSamples>
    class BaseSpectrum;
    class PrincipalSpectrum;
    class RGBSpectrum;

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

    class SamplerIntegrator;
    class PathIntegrator;

    class Bvh;
    class RayTraverser;
    struct PrimitiveIntersector;

    // Global Inline Functions
    inline uint32_t
    FloatToBits(float f)
    {
        uint32_t ui;
        memcpy(&ui, &f, sizeof(float));
        return ui;
    }

    inline float BitsToFloat(uint32_t ui)
    {
        float f;
        memcpy(&f, &ui, sizeof(uint32_t));
        return f;
    }

} // namespace filianore

#endif