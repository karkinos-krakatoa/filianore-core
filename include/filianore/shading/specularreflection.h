#ifndef _SPECULAR_REFLECTION_H
#define _SPECULAR_REFLECTION_H

#include "../core/bxdf.h"

namespace filianore
{

    class SpecularReflection : public BxDF
    {
    public:
        SpecularReflection(const PrincipalSpectrum &_R)
            : R(_R), BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR))
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