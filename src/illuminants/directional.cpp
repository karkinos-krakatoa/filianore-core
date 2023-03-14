#include "filianore/illuminants/directional.h"
#include "filianore/maths/mathutils.h"

namespace filianore {

DirectionalIlluminant::DirectionalIlluminant(const Transform &_lightToWorld, const Vector3f &_dirIllum,
                                             const PrincipalSpectrum &_color, float _intensity, const PrincipalSpectrum &_shadowColor)
    : Illuminant(_lightToWorld, (int)IlluminantType::DeltaDirectional, 1, 0.f, _shadowColor), color(_color), intensity(_intensity) {
    worldCenter = Vector3f(0.f);
    worldRadius = 1000.f;

    dirIllum = normalize(-_lightToWorld.vector_transform(_dirIllum));
}

PrincipalSpectrum DirectionalIlluminant::sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                                   VisibilityEvaluator *visEval) const {
    *wi = dirIllum;
    *pdf = 1.f;

    Vector3f pOutside = isect.p + dirIllum * (2.f * worldRadius);
    *visEval = VisibilityEvaluator(isect, Interaction(pOutside, isect.time));

    return (color * intensity);
}

PrincipalSpectrum DirectionalIlluminant::power() const {
    return PrincipalSpectrum(PI * worldRadius * worldRadius);
}

void DirectionalIlluminant::prepare_illuminant(const Scene &scene) {
}

float DirectionalIlluminant::pdf_li(const Interaction &ref, const Vector3f &wi) const {
    return 0.f;
}

} // namespace filianore