#ifndef _MICROFACET_REFLECTION_H
#define _MICROFACET_REFLECTION_H

#include "../../core/bxdf.h"

namespace filianore
{

    class MicrofacetReflectionBRDF : public BxDF
    {
    public:
        MicrofacetReflectionBRDF(const std::shared_ptr<MicrofacetDistribution> &_distribution,
                                 const std::shared_ptr<Fresnel> &_fresnel, const PrincipalSpectrum &_R, const float alphax, const float alphay);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum R;
        std::shared_ptr<MicrofacetDistribution> distribution;
        std::shared_ptr<Fresnel> fresnel;
    };
}

#endif