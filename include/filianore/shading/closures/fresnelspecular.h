#ifndef _FRESNEL_SPECULAR_BRDF_H
#define _FRESNEL_SPECULAR_BRDF_H

#include "../../core/bxdf.h"

namespace filianore {

class FresnelSpecularBXDF : public BxDF {
public:
    FresnelSpecularBXDF(const PrincipalSpectrum &_R, const PrincipalSpectrum &_T, float _etaA, float _etaB);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum R, T;
    const float etaA, etaB;
    std::shared_ptr<Fresnel> fresnel;
};
} // namespace filianore

#endif