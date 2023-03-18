#ifndef _DIFFUSE_BTDF_H
#define _DIFFUSE_BTDF_H

#include "../../core/bxdf.h"

namespace filianore {

class DiffuseTransmission : public BxDF {
public:
    DiffuseTransmission(const PrincipalSpectrum &_T, float _weight, float sigma);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    std::unique_ptr<BxDF> diffuseBxdf;
};

} // namespace filianore

#endif