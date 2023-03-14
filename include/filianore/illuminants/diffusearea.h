#ifndef _ILLUMINANT_DIFFUSE_AREA_H
#define _ILLUMINANT_DIFFUSE_AREA_H

#include "../core/illuminant.h"

namespace filianore {

class DiffuseAreaIlluminant : public AreaIlluminant {
public:
    DiffuseAreaIlluminant(const Transform &_lightToWorld, const PrincipalSpectrum &_color, float _intensity,
                          short _decayRate, const PrincipalSpectrum &_shadowColor, const std::shared_ptr<Shape> &_shape);

    PrincipalSpectrum sample_li(const Interaction &isect, const Vector2f &u, Vector3f *wi, float *pdf,
                                VisibilityEvaluator *visEval) const;

    PrincipalSpectrum power() const;

    void prepare_illuminant(const Scene &scene);

    float pdf_li(const Interaction &ref, const Vector3f &wi) const;

    PrincipalSpectrum L(const Interaction &isect, const Vector3f &w) const;

private:
    const float intensity;
    const PrincipalSpectrum color;
    const std::shared_ptr<Shape> shape;
    const float area;
};

} // namespace filianore

#endif