#ifndef _LAMBERT_BRDF_H
#define _LAMBERT_BRDF_H

#include "../../core/bxdf.h"

namespace filianore
{

    class LambertBRDF : public BxDF
    {
    public:
        LambertBRDF(const PrincipalSpectrum &_R)
            : R(_R), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE))
        {
        }

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum R;
    };

} // namespace filianore

#endif