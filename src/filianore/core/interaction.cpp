#include "interaction.h"
#include "../core/primitive.h"
#include "../illuminants/illuminant.h"

namespace filianore {

SurfaceInteraction::SurfaceInteraction(float _t, const Vector3f &_p, const Vector2f &_uv,
                                       const Vector3f &_dpdu, const Vector3f &_dpdv,
                                       const Vector3f &_dndu, const Vector3f &_dndv,
                                       const Vector3f &_wo, const Shape *_shape, float _time)
    : Interaction(_t, _p, normalize(cross(_dpdu, _dpdv)), _wo, _time), uv(_uv), shape(_shape),
      dpdu(_dpdu), dpdv(_dpdv), dndu(_dndu), dndv(_dndv) {
    Shading.n = n;
    Shading.dpdu = _dpdu;
    Shading.dpdv = _dpdv;
    Shading.dndu = _dndu;
    Shading.dndv = _dndv;
}

void SurfaceInteraction::set_shading_geometry(const Vector3f &_dpdu, const Vector3f &_dpdv,
                                              const Vector3f &_dndu, const Vector3f &_dndv, bool orientationIsAuthoritative) {
    Shading.n = normalize(cross(_dpdu, _dpdv));

    if (orientationIsAuthoritative)
        n = face_forward(n, Shading.n);
    else
        Shading.n = face_forward(Shading.n, n);

    Shading.dpdu = _dpdu;
    Shading.dpdv = _dpdv;
    Shading.dndu = _dndu;
    Shading.dndv = _dndv;
}

PrincipalSpectrum SurfaceInteraction::le(const Vector3f &w) const {
    const AreaIlluminant *areaIllum = primitive->get_area_illuminant();
    return areaIllum ? areaIllum->L(*this, w) : PrincipalSpectrum(0.f);
}

void SurfaceInteraction::compute_scattering_functions(const Ray &ray) {
    if (primitive) {
        primitive->compute_scattering_functions(this);
    }
}

} // namespace filianore