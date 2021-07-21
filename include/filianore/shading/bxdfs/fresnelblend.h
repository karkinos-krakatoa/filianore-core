#ifndef _FRESNEL_BLEND_H
#define _FRESNEL_BLEND_H

#include "../../core/bxdf.h"

namespace filianore
{

    class FresnelBlend : public BxDF
    {
    public:
        FresnelBlend(const PrincipalSpectrum &_Rd, const PrincipalSpectrum &_Rs,
                     const std::shared_ptr<MicrofacetDistribution> &_distribution);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum SchlickFresnel(float cosTheta) const
        {
            auto pow5 = [](float v)
            { return (v * v) * (v * v) * v; };
            return Rs + pow5(1 - cosTheta) * (PrincipalSpectrum(1.f) - Rs);
        }

    private:
        const PrincipalSpectrum Rd, Rs;
        std::shared_ptr<MicrofacetDistribution> distribution;
    };
}

#endif