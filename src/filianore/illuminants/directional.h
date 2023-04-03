#ifndef _ILLUMINANT_DIRECTIONAL_H
#define _ILLUMINANT_DIRECTIONAL_H

#include "illuminant.h"

namespace filianore {

class DirectionalIlluminant : public Illuminant {
public:
    DirectionalIlluminant(const Transform &_lightToWorld, const Vector3f &_dirIllum,
                          const PrincipalSpectrum &_color, float _intensity, const PrincipalSpectrum &_shadowColor);

    PrincipalSpectrum sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                VisibilityEvaluator *visEval) const;

    PrincipalSpectrum power() const;

    void prepare_illuminant(const Scene &scene);

    float pdf_li(const Interaction &ref, const Vector3f &wi) const;

private:
    Vector3f dirIllum;
    const float intensity;
    const PrincipalSpectrum color;
    Vector3f worldCenter;
    float worldRadius;
};

} // namespace filianore

#endif