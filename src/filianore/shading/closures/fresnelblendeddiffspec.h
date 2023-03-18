#ifndef _FRESNEL_BLENDED_DIFFUSE_SPECULAR_BRDF_H
#define _FRESNEL_BLENDED_DIFFUSE_SPECULAR_BRDF_H

#include "../../core/bxdf.h"
#include "../bxdfs/orennayar.h"

namespace filianore {

class FresnelBlendedDiffuseSpecularBRDF : public BxDF {
public:
    FresnelBlendedDiffuseSpecularBRDF(const PrincipalSpectrum &_kd, float _kdWeight, float _kdRoughness,

                                      const PrincipalSpectrum &_ks, float _ksWeight, float _ro,

                                      const std::shared_ptr<Fresnel> &_fresnel,
                                      const std::shared_ptr<MicrofacetDistribution> &_distribution);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum kd;
    float kdWeight;
    float kdRoughness;

    const PrincipalSpectrum ks;
    float ksWeight;
    float ro;

    const std::shared_ptr<Fresnel> fresnel;
    const std::shared_ptr<MicrofacetDistribution> distribution;

    std::shared_ptr<OrenNayarBRDF> orenNayarBrdf;
};
} // namespace filianore

#endif