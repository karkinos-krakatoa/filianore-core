#ifndef _CLEARCOAT_BRDF_H
#define _CLEARCOAT_BRDF_H

#include "bxdf.h"

namespace filianore {

class ClearcoatReflectionBRDF : public BxDF {
public:
    ClearcoatReflectionBRDF(const PrincipalSpectrum &_R, float _ior, float _weight, float _gloss);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum R;
    const float weight, ior;
    float gloss;
    std::shared_ptr<MicrofacetDistribution> distribution;
    std::shared_ptr<Fresnel> fresnel;
};
} // namespace filianore

#endif