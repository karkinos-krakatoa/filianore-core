#ifndef _LAMBERT_BRDF_H
#define _LAMBERT_BRDF_H

#include "bxdf.h"

namespace filianore {

class LambertBRDF : public BxDF {
public:
    LambertBRDF(const PrincipalSpectrum &_R, float _weight)
        : R(_R), weight(_weight), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)) {
    }

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum R;
    const float weight;
};

} // namespace filianore

#endif