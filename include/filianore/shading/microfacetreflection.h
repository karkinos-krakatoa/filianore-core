#ifndef _MICROFACET_REFLECTION_H
#define _MICROFACET_REFLECTION_H

#include "../core/bxdf.h"

namespace filianore
{

    class MicrofacetReflection : public BxDF
    {
    public:
        MicrofacetReflection(const PrincipalSpectrum &_R,
                             const std::shared_ptr<MicrofacetDistribution> &_distribution,
                             const std::shared_ptr<Fresnel> &_fresnel)
            : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(_R),
              distribution(_distribution), fresnel(_fresnel)
        {
        }

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum R;
        const std::shared_ptr<MicrofacetDistribution> distribution;
        const std::shared_ptr<Fresnel> fresnel;
    };
}

#endif