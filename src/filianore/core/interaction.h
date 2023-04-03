#ifndef _INTERACTION_H
#define _INTERACTION_H

#include "../color/principalspectrum.h"
#include "../maths/affine.h"
#include "../maths/geometry.h"
#include "elemental.h"

namespace filianore {

inline Vector3f offset_point_origin(const Vector3f &p, const Vector3f &pError,
                                    const Vector3f &n, const Vector3f &w) {
    float d = dot(abs(n), pError);
    Vector3f offset = n * d;

    if (dot(w, n) < 0) {
        offset = -offset;
    }

    Vector3f po = p + offset;

    for (int i = 0; i < 3; ++i) {
        if (offset[i] > 0) {
            po[i] = next_float_up<float>(po[i]);
        } else if (offset[i] < 0) {
            po[i] = next_float_down<float>(po[i]);
        }
    }

    return po;
}

struct Interaction {
    Interaction() : t(0), time(0) {}

    Interaction(float _t, const Vector3f &_p, const Vector3f &_wo, float _time)
        : t(_t), p(_p), time(_time) {
        wo = _wo;
        wo = normalize(wo);
    }

    Interaction(float _t, const Vector3f &_p, const Vector3f &_n, const Vector3f &_wo, float _time)
        : t(_t), p(_p), n(_n), time(_time) {
        wo = _wo;
        wo = normalize(wo);
    }

    Interaction(const Vector3f &_p, float _time)
        : t(0.f), p(_p), time(_time) {
    }

    bool is_surface_interaction() const {
        return (n.x != 0.f || n.y != 0.f || n.z != 0.f);
    }

    bool is_medium_interaction() const {
        return !is_surface_interaction();
    }

    Ray kindle_ray(const Vector3f &d) const {
        Vector3f o = offset_point_origin(p, pError, n, d);
        return Ray(o, d, SHADOW_EPSILON, INFINITI);
    }

    Ray kindle_ray_to(const Vector3f &p2) const {
        Vector3f d = p2 - p;
        Vector3f o = offset_point_origin(p, pError, n, d);
        return Ray(o, d, SHADOW_EPSILON, 1.f - SHADOW_EPSILON);
    }

    float time;
    float t;
    Vector3f p;
    Vector3f pError;
    Vector3f wo;
    Vector3f n;
};

class SurfaceInteraction : public Interaction {
public:
    SurfaceInteraction() {}

    SurfaceInteraction(float _t, const Vector3f &_p, const Vector2f &_uv,
                       const Vector3f &_dpdu, const Vector3f &_dpdv,
                       const Vector3f &_dndu, const Vector3f &_dndv,
                       const Vector3f &_wo, const Shape *_shape, float _time);

    PrincipalSpectrum le(const Vector3f &w) const;

    void compute_scattering_functions(const Ray &ray);
    void set_shading_geometry(const Vector3f &_dpdu, const Vector3f &_dpdv,
                              const Vector3f &_dndu, const Vector3f &_dndv, bool orientationIsAuthoritative);

    Vector2f uv;
    Vector3f dpdu, dpdv;
    Vector3f dndu, dndv;

    struct
    {
        Vector3f n;
        Vector3f dpdu, dpdv;
        Vector3f dndu, dndv;
    } Shading;

    const Shape *shape = nullptr;
    const Primitive *primitive = nullptr;
    std::shared_ptr<BSDF> bsdf = nullptr;
};

} // namespace filianore

#endif