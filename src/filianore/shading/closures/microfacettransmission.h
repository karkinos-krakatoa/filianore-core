#ifndef _MICROFACET_TRANSMISSION_H
#define _MICROFACET_TRANSMISSION_H

#include "../bxdfs/bxdf.h"

namespace filianore {

class MicrofacetTransmissionBRDF : public BxDF {
public:
    MicrofacetTransmissionBRDF(const PrincipalSpectrum &_kr, float _krweight,
                               const PrincipalSpectrum &_kt, float _ktweight,
                               float _etaA, float _etaB,
                               const std::shared_ptr<MicrofacetDistribution> &_distribution);

    PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const;

    PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const;

    float pdf(const Vector3f &wo, const Vector3f &wi) const;

private:
    const PrincipalSpectrum kr;
    const PrincipalSpectrum kt;
    const float krweight, ktweight;
    const float etaA, etaB;
    const std::shared_ptr<MicrofacetDistribution> distribution;
    std::shared_ptr<Fresnel> fresnel;
};
} // namespace filianore

#endif