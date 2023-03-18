#include "spot.h"
#include "../maths/mathutils.h"

namespace filianore {

SpotIlluminant::SpotIlluminant(const Transform &_lightToWorld, const Vector3f &_dirIllum,
                               float _coneAngle, float _penumbraAngle, bool angleInRadians, bool usehalfAngles, const PrincipalSpectrum &_color,
                               float _intensity, short _decayRate, const PrincipalSpectrum &_shadowColor)
    : Illuminant(_lightToWorld, (int)IlluminantType::DeltaPoint, 1, _decayRate, _shadowColor), color(_color), intensity(_intensity) {
    float coneAngle = angleInRadians ? _coneAngle : radians<float>(_coneAngle);
    coneAngle = usehalfAngles ? coneAngle / 2.f : coneAngle;
    cosConeAngle = std::cos(coneAngle);

    float penumbraAngle = angleInRadians ? _penumbraAngle : radians<float>(_penumbraAngle);
    penumbraAngle = usehalfAngles ? penumbraAngle / 2.f : penumbraAngle;
    cosPenumbraAngle = std::cos(penumbraAngle);

    posIllum = _lightToWorld.point_transform(Vector3f(0.f));
    dirIllum = _lightToWorld.vector_transform(_dirIllum);
}

PrincipalSpectrum SpotIlluminant::sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                            VisibilityEvaluator *visEval) const {
    *wi = normalize(posIllum - isect.p);
    *pdf = 1.f;

    *visEval = VisibilityEvaluator(isect, Interaction(posIllum, isect.time));

    return (color * intensity * Falloff(Vector3f(-wi->x, -wi->y, -wi->z))) / evaluate_decay_rate(posIllum - isect.p);
}

PrincipalSpectrum SpotIlluminant::power() const {
    return PrincipalSpectrum(intensity * 2 * PI * (1.f - .5f * (cosConeAngle + cosPenumbraAngle)));
}

void SpotIlluminant::prepare_illuminant(const Scene &scene) {
}

float SpotIlluminant::pdf_li(const Interaction &ref, const Vector3f &wi) const {
    return 0.f;
}

float SpotIlluminant::Falloff(const Vector3f &w) const {
    Vector3f wNml = w;
    Vector3f dirNml = dirIllum;

    float cosTheta = dot(normalize(wNml), normalize(dirNml));

    if (cosTheta < cosConeAngle)
        return 0.f;
    if (cosTheta > cosPenumbraAngle)
        return 1.f;

    float t = filianore::clamp((cosTheta - cosConeAngle) / (cosPenumbraAngle - cosConeAngle), 0.f, 1.f);
    return t * t * (3.f - 2.f * t);
}

} // namespace filianore