#ifndef _DIFFUSE_BTDF_H
#define _DIFFUSE_BTDF_H

#include "../../core/bxdf.h"

namespace filianore
{

    class DiffuseTransmission : public BxDF
    {
    public:
        DiffuseTransmission(const PrincipalSpectrum &_T, float _weight, float sigma);

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        std::unique_ptr<BxDF> diffuseBxdf;
    };

} // namespace filianore

#endif