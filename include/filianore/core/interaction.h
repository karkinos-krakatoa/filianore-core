#ifndef _INTERACTION_H
#define _INTERACTION_H

#include "elemental.h"
#include "ray.h"
#include "../maths/transform.h"

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
            return true;
        }

        bool IsMediumInteraction() const
        {
            return !IsSurfaceInteraction();
        }

        Ray KindleRay(const StaticArray<float, 3> &d) const
        {
            StaticArray<float, 3> o = p + d * Epsilon<float>;
            return Ray(o, d, Epsilon<float>, Infinity<float>(), time);
        }

        Ray KindleRayTo(const StaticArray<float, 3> &p2) const
        {
            StaticArray<float, 3> d = p2 - p;
            StaticArray<float, 3> o = p + d * Epsilon<float>;
            return Ray(o, d, Epsilon<float>, 1.f - Epsilon<float>, time);
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
            : Interaction(_t, _p, _n, _wo, _time), uv(_uv)
        {
        }

        Color Le(const StaticArray<float, 3> &w) const;

        void ComputeScatteringFunctions(const Ray &ray);

        StaticArray<float, 2> uv;
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        std::shared_ptr<BSDF> bsdf = nullptr;
    };

} // namespace filianore

#endif