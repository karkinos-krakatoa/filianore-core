#ifndef _MICROFACET_REFLECTION_H
#define _MICROFACET_REFLECTION_H

#include "bxdf.h"

namespace filianore {

class MicrofacetReflectionBRDF : public BxDF {
public:
    MicrofacetReflectionBRDF(const std::shared_ptr<MicrofacetDistribution> &_distribution,
                             const std::shared_ptr<Fresnel> &_fresnel, const PrincipalSpectrum &_R, float _weight);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum R;
    const float weight;
    std::shared_ptr<MicrofacetDistribution> distribution;
    std::shared_ptr<Fresnel> fresnel;
};
} // namespace filianore

#endif