#ifndef _LAMBERT_REFLECTION_H
#define _LAMBERT_REFLECTION_H

#include "../core/bxdf.h"

namespace filianore
{

    class LambertReflection : public BxDF
    {
    public:
        LambertReflection(const Spectrum<float> &_R)
            : R(_R), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE))
        {
        }

        Spectrum<float> Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

        Spectrum<float> Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const Spectrum<float> R;
    };

} // namespace filianore

#endif