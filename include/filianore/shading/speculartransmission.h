#ifndef _SPECULAR_TRANSMISSION_H
#define _SPECULAR_TRANSMISSION_H

#include "../core/bxdf.h"
#include "fresneldielectric.h"

namespace filianore
{

    class SpecularTransmission : public BxDF
    {
    public:
        SpecularTransmission(const PrincipalSpectrum &_T, float _etaA, float _etaB)
            : T(_T), etaA(_etaA), etaB(_etaB), fresnel(_etaA, _etaB), BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR))
        {
        }

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum T;
        const FresnelDielectric fresnel;
        const float etaA, etaB;
    };

} // namespace filianore

#endif