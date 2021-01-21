#ifndef _INTERACTION_H
#define _INTERACTION_H

#include "elemental.h"
#include "ray.h"
#include "../maths/transform.h"
#include "../color/rgb.h"

namespace filianore
{

    struct Interaction
    {
        Interaction() : t(0), time(0) {}

        Interaction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 3> &_wo, float _time)
            : t(_t), p(_p), wo(_wo), time(_time)
        {
        }

        Interaction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 3> &_n, const StaticArray<float, 3> &_wo, float _time)
            : t(_t), p(_p), n(_n), wo(_wo), time(_time)
        {
        }

        Interaction(const StaticArray<float, 3> &_p, float _time)
            : t(0.f), p(_p), time(_time)
        {
        }

        bool IsSurfaceInteraction() const
        {
            return (n.x() != 0.f || n.y() != 0.f || n.z() != 0.f);
        }

        bool IsMediumInteraction() const
        {
            return !IsSurfaceInteraction();
        }

        Ray KindleRay(const StaticArray<float, 3> &d) const
        {
            return Ray(p + n * Epsilon<float>, d, Epsilon<float>, Infinity<float>());
        }

        Ray KindleRayTo(const StaticArray<float, 3> &p2) const
        {
            return Ray(p + n * Epsilon<float>, p2 - p, Epsilon<float>, 1.f - Epsilon<float>);
        }

        float time;
        float t;
        StaticArray<float, 3> p;
        StaticArray<float, 3> wo;
        StaticArray<float, 3> n;
    };

    class SurfaceInteraction : public Interaction
    {
    public:
        SurfaceInteraction() {}

        SurfaceInteraction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 3> &_n, const StaticArray<float, 2> &_uv,
                           const StaticArray<float, 3> &_wo, const Shape *_shape, float _time)
            : Interaction(_t, _p, _n, _wo, _time), uv(_uv), shape(_shape)
        {
        }

        RGBSpectrum Le(const StaticArray<float, 3> &w) const;

        void ComputeScatteringFunctions(const Ray &ray);

        StaticArray<float, 3> ns;
        StaticArray<float, 2> uv;
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        std::shared_ptr<BSDF> bsdf = nullptr;
    };

} // namespace filianore

#endif