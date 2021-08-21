#ifndef _CLEARCOAT_BRDF_H
#define _CLEARCOAT_BRDF_H

#include "../../core/bxdf.h"

namespace filianore
{

    class ClearcoatReflectionBRDF : public BxDF
    {
    public:
        ClearcoatReflectionBRDF(const PrincipalSpectrum &_R, float _ior, float _weight, float _gloss);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum R;
        const float weight, ior;
        float gloss;
        std::shared_ptr<MicrofacetDistribution> distribution;
        std::shared_ptr<Fresnel> fresnel;
    };
}

#endif