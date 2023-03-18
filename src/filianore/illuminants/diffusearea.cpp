#include "diffusearea.h"
#include "../core/shape.h"
#include "../maths/mathutils.h"

namespace filianore {

DiffuseAreaIlluminant::DiffuseAreaIlluminant(const Transform &_lightToWorld, const PrincipalSpectrum &_color, float _intensity,
                                             short _decayRate, const PrincipalSpectrum &_shadowColor, const std::shared_ptr<Shape> &_shape)
    : AreaIlluminant(_lightToWorld, _decayRate, _shadowColor), color(_color), intensity(_intensity), shape(_shape), area(shape->area()) {
}

PrincipalSpectrum DiffuseAreaIlluminant::sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf, VisibilityEvaluator *visEval) const {
    Interaction ist = shape->sample(isect, u, pdf);

    if (*pdf == 0 || (ist.p - isect.p).length_squared() == 0) {
        *pdf = 0.f;
        return PrincipalSpectrum(0.f);
    }

    *wi = normalize(ist.p - isect.p);
    *visEval = VisibilityEvaluator(isect, ist);

    return L(ist, Vector3f(-wi->x, -wi->y, -wi->z));
}

PrincipalSpectrum DiffuseAreaIlluminant::power() const {
    return color * intensity * area * PI;
}

void DiffuseAreaIlluminant::prepare_illuminant(const Scene &scene) {
}

float DiffuseAreaIlluminant::pdf_li(const Interaction &ref, const Vector3f &wi) const {
    return shape->pdf(ref, wi);
}

PrincipalSpectrum DiffuseAreaIlluminant::L(const Interaction &isect, const Vector3f &w) const {
    return dot(isect.n, w) > 0.f ? color * intensity : PrincipalSpectrum(0.f);
}

} // namespace filianore