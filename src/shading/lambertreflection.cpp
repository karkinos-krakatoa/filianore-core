#include "filianore/shading/lambertreflection.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    Spectrum<float> LambertReflection::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return R / Pi<float>;
    }

    Spectrum<float> LambertReflection::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        *wi = CosineHemisphereSample(sample);

        if (wo.z() < 0)
        {
            wi->params[2] *= -1;
        }

        *pdf = Pdf(wo, *wi);
        *sampledType = this->bxDFType;

        return Evaluate(wo, *wi);
    }

    float LambertReflection::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return SameHemisphere(wo, wi) ? AbsCosTheta(wi) / Pi<float> : 0.f;
    }

} // namespace filianore