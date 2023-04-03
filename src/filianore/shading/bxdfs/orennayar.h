#ifndef _OREN_NAYAR_BRDF_H
#define _OREN_NAYAR_BRDF_H

#include "bxdf.h"

namespace filianore {

class OrenNayarBRDF : public BxDF {
public:
    OrenNayarBRDF(const PrincipalSpectrum &_R, float _weight, float sigma);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum R;
    const float weight;
    float A, B;
};

} // namespace filianore

#endif