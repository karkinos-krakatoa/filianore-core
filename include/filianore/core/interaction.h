#ifndef _INTERACTION_H
#define _INTERACTION_H

#include "elemental.h"
#include "ray.h"
#include "../maths/transform.h"
#include "../color/principalspectrum.h"

namespace filianore
{

    inline StaticArray<float, 3> OffsetPointOrigin(const StaticArray<float, 3> &p, const StaticArray<float, 3> &pError,
                                                   const StaticArray<float, 3> &n, const StaticArray<float, 3> w)
    {
        float d = Dot(Abs(n), pError);
        StaticArray<float, 3> offset = n * d;

        if (Dot(w, n) < 0)
        {
            offset = offset.Neg();
        }

        StaticArray<float, 3> po = p + offset;

        for (int i = 0; i < 3; ++i)
        {
            if (offset.params[i] > 0)
            {
                po.params[i] = NextFloatUp<float>(po.params[i]);
            }
            else if (offset.params[i] < 0)
            {
                po.params[i] = NextFloatDown<float>(po.params[i]);
            }
        }

        return po;
    }

    struct Interaction
    {
        Interaction() : t(0), time(0) {}

        Interaction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 3> &_wo, float _time)
            : t(_t), p(_p), time(_time)
        {
            wo = _wo;
            wo = wo.Normalize();
        }

        Interaction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 3> &_n, const StaticArray<float, 3> &_wo, float _time)
            : t(_t), p(_p), n(_n), time(_time)
        {
            wo = _wo;
            wo = wo.Normalize();
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
            StaticArray<float, 3> o = OffsetPointOrigin(p, pError, n, d);
            return Ray(o, d, Epsilon<float>, Infinity<float>());
        }

        Ray KindleRayTo(const StaticArray<float, 3> &p2) const
        {
            StaticArray<float, 3> d = p2 - p;
            StaticArray<float, 3> o = OffsetPointOrigin(p, pError, n, d);
            return Ray(o, d, Epsilon<float>, 1.f - Epsilon<float>);
        }

        float time;
        float t;
        StaticArray<float, 3> p;
        StaticArray<float, 3> pError;
        StaticArray<float, 3> wo;
        StaticArray<float, 3> n;
    };

    class SurfaceInteraction : public Interaction
    {
    public:
        SurfaceInteraction() {}

        SurfaceInteraction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 2> &_uv,
                           const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                           const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv,
                           const StaticArray<float, 3> &_wo, const Shape *_shape, float _time);

        PrincipalSpectrum Le(const StaticArray<float, 3> &w) const;

        void ComputeScatteringFunctions(const Ray &ray);
        void SetShadingGeometry(const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                                const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv, bool orientationIsAuthoritative);

        StaticArray<float, 2> uv;
        StaticArray<float, 3> dpdu, dpdv;
        StaticArray<float, 3> dndu, dndv;

        struct
        {
            StaticArray<float, 3> n;
            StaticArray<float, 3> dpdu, dpdv;
            StaticArray<float, 3> dndu, dndv;
        } Shading;

        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        std::shared_ptr<BSDF> bsdf = nullptr;
    };

} // namespace filianore

#endif