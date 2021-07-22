#ifndef _PLASTIC_BRDF_H
#define _PLASTIC_BRDF_H

#include "../../core/bxdf.h"

namespace filianore
{

    class PlasticBRDF : public BxDF
    {
    public:
        PlasticBRDF(float _kdWeight, const PrincipalSpectrum &_kd,
                    float _ksWeight, const PrincipalSpectrum &_ks, float _ro,
                    const std::shared_ptr<MicrofacetDistribution> &_distribution);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum SchlickFresnel(float cosTheta) const
        {
            auto pow5 = [](float v)
            { return (v * v) * (v * v) * v; };
            float r = ro + pow5(1.f - cosTheta) * (1.f - ro);
            return PrincipalSpectrum(r);
        }

    private:
        const float ro;
        const float kdWeight, ksWeight;
        const PrincipalSpectrum kd, ks;
        std::shared_ptr<MicrofacetDistribution> distribution;
    };
}

#endif