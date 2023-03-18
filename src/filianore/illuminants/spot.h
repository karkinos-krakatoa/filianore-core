#ifndef _ILLUMINANT_SPOT_H
#define _ILLUMINANT_SPOT_H

#include "../core/illuminant.h"

namespace filianore {

class SpotIlluminant : public Illuminant {
public:
    SpotIlluminant(const Transform &_lightToWorld, const Vector3f &_dirIllum,
                   float _coneAngle, float _penumbraAngle, bool angleInRadians, bool usehalfAngles, const PrincipalSpectrum &_color,
                   float _intensity, short _decayRate, const PrincipalSpectrum &_shadowColor);

    PrincipalSpectrum sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                VisibilityEvaluator *visEval) const;

    PrincipalSpectrum power() const;

    void prepare_illuminant(const Scene &scene);

    float pdf_li(const Interaction &ref, const Vector3f &wi) const;

private:
    float Falloff(const Vector3f &w) const;

    Vector3f posIllum;
    Vector3f dirIllum;
    const float intensity;
    const PrincipalSpectrum color;
    float cosConeAngle, cosPenumbraAngle;
};

} // namespace filianore

#endif