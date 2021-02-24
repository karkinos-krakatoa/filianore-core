#ifndef _INTERACTION_H
#define _INTERACTION_H

#include "elemental.h"
#include "ray.h"
#include "../maths/transform.h"
#include "../color/principalspectrum.h"

namespace filianore
{

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

        SurfaceInteraction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 2> &_uv,
                           const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                           const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv,
                           const StaticArray<float, 3> &_wo, const Shape *_shape, float _time)
            : Interaction(_t, _p, Cross(_dpdu, _dpdv).Normalize(), _wo, _time),
              dpdu(_dpdu), dpdv(_dpdv), dndu(_dndu), dndv(_dndv),
              uv(_uv), shape(_shape)
        {
            shading.n = n;
            shading.dpdu = _dpdu;
            shading.dpdv = _dpdv;
            shading.dndu = _dndu;
            shading.dndv = _dndv;
        }

        PrincipalSpectrum Le(const StaticArray<float, 3> &w) const;

        void ComputeScatteringFunctions(const Ray &ray);
        void SetShadingGeometry(const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                                const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv,
                                bool orientationIsAuthoritative);

        struct
        {
            StaticArray<float, 3> n;
            StaticArray<float, 3> dpdu, dpdv;
            StaticArray<float, 3> dndu, dndv;
        } shading;

        StaticArray<float, 2> uv;
        StaticArray<float, 3> dpdu, dpdv;
        StaticArray<float, 3> dndu, dndv;
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        std::shared_ptr<BSDF> bsdf = nullptr;
    };

} // namespace filianore

#endif