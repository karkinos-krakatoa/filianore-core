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

        bool IsSurfaceInteraction() const
        {
            //return n != StaticArray<float, 3>();
            return true;
        }

        bool IsMediumInteraction() const
        {
            return !IsSurfaceInteraction();
        }

        Ray KindleRay(const StaticArray<float, 3> &d) const
        {
            StaticArray<float, 3> o = p + n * Epsilon<float>;
            return Ray(o, d, Epsilon<float>, Infinity<float>(), time);
        }

        Ray KindleRayTo(const StaticArray<float, 3> &p2) const
        {
            StaticArray<float, 3> d = p2 - p;
            StaticArray<float, 3> o = p + n * Epsilon<float>;
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

        ~SurfaceInteraction() {}

        SurfaceInteraction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 3> &_n, const StaticArray<float, 2> &_uv,
                           const StaticArray<float, 3> &_wo, const Shape *_shape, float _time)
            : Interaction(_t, _p, _n, _wo, _time), uv(_uv)
        {
        }

        /*void ComputeScatteringFunctions(const Ray<T, N>& ray, MemoryArena& arena, bool allowMultipleLobes = false,
			TransportMode mode = TransportMode::Radiance);

		Spectrum<T> Le(const Vector<T, N>& w) const;*/

        StaticArray<float, 2> uv;
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        std::shared_ptr<BSDF> bsdf = nullptr;
    };

} // namespace filianore

#endif