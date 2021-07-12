#ifndef _OREN_NAYAR_BRDF_H
#define _OREN_NAYAR_BRDF_H

#include "../../core/bxdf.h"

namespace filianore
{

    class OrenNayarBRDF : public BxDF
    {
    public:
        OrenNayarBRDF(const PrincipalSpectrum &_R, float sigma);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const PrincipalSpectrum R;
        float A, B;
    };

} // namespace filianore

#endif