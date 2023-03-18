#include "point.h"
#include "../maths/mathutils.h"

namespace filianore {

PointIlluminant::PointIlluminant(const Transform &_lightToWorld, const PrincipalSpectrum &_color, float _intensity, short _decayRate, const PrincipalSpectrum &_shadowColor)
    : Illuminant(_lightToWorld, (int)IlluminantType::DeltaPoint, 1, _decayRate, _shadowColor), color(_color), intensity(_intensity) {
    posIllum = Vector3f(0.f);
    posIllum = _lightToWorld.point_transform(posIllum);
}

PrincipalSpectrum PointIlluminant::sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                             VisibilityEvaluator *visEval) const {
    *wi = normalize(posIllum - isect.p);
    *pdf = 1.f;

    *visEval = VisibilityEvaluator(isect, Interaction(posIllum, isect.time));

    return (color * intensity) / evaluate_decay_rate(posIllum - isect.p);
}

PrincipalSpectrum PointIlluminant::power() const {
    return PrincipalSpectrum(intensity * 4.f * PI);
}

void PointIlluminant::prepare_illuminant(const Scene &scene) {
}

float PointIlluminant::pdf_li(const Interaction &ref, const Vector3f &wi) const {
    return 0.f;
}

} // namespace filianore