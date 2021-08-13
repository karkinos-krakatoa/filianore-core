#ifndef _FRESNEL_SPECULAR_BRDF_H
#define _FRESNEL_SPECULAR_BRDF_H

#include "../../core/bxdf.h"

namespace filianore
{

    class FresnelSpecularBXDF : public BxDF
    {
    public:
        FresnelSpecularBXDF(const PrincipalSpectrum &_R, const PrincipalSpectrum &_T, float _etaA, float _etaB);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum R, T;
        const float etaA, etaB;
        std::shared_ptr<Fresnel> fresnel;
    };
}

#endif