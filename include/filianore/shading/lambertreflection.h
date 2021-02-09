#ifndef _LAMBERT_REFLECTION_H
#define _LAMBERT_REFLECTION_H

#include "../core/bxdf.h"

namespace filianore
{

    class LambertReflection : public BxDF
    {
    public:
        LambertReflection(const RGBSpectrum &_R)
            : R(_R), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE))
        {
        }

        RGBSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        RGBSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const RGBSpectrum R;
    };

} // namespace filianore

#endif