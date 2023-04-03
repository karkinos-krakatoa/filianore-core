#ifndef _ILLUMINANT_POINT_H
#define _ILLUMINANT_POINT_H

#include "illuminant.h"

namespace filianore {

class PointIlluminant : public Illuminant {
public:
    PointIlluminant(const Transform &_lightToWorld, const PrincipalSpectrum &_color, float _intensity, short _decayRate, const PrincipalSpectrum &_shadowColor);

    PrincipalSpectrum sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                VisibilityEvaluator *visEval) const;

    PrincipalSpectrum power() const;

    void prepare_illuminant(const Scene &scene);

    float pdf_li(const Interaction &ref, const Vector3f &wi) const;

private:
    Vector3f posIllum;
    const float intensity;
    const PrincipalSpectrum color;
};

} // namespace filianore

#endif