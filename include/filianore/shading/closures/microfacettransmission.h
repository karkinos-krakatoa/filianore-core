#ifndef _MICROFACET_TRANSMISSION_H
#define _MICROFACET_TRANSMISSION_H

#include "../../core/bxdf.h"

namespace filianore
{

    class MicrofacetTransmissionBRDF : public BxDF
    {
    public:
        MicrofacetTransmissionBRDF(const PrincipalSpectrum &_kr, float _krweight,
                                   const PrincipalSpectrum &_kt, float _ktweight,
                                   float _etaA, float _etaB,
                                   const std::shared_ptr<MicrofacetDistribution> &_distribution);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum kr;
        const PrincipalSpectrum kt;
        const float krweight, ktweight;
        const float etaA, etaB;
        const std::shared_ptr<MicrofacetDistribution> distribution;
        std::shared_ptr<Fresnel> fresnel;
    };
}

#endif