#include "filianore/shading/bxdfs/lambert.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    PrincipalSpectrum LambertBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return R * InvPi<float>;
    }

    PrincipalSpectrum LambertBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        *wi = CosineHemisphereSample(sample);

        if (wo.z() < 0)
        {
            wi->params[2] *= -1.f;
        }

        *pdf = Pdf(wo, *wi);
        *sampledType = this->bxDFType;

        return Evaluate(wo, *wi);
    }

    float LambertBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return SameHemisphere(wo, wi) ? AbsCosTheta(wi) * InvPi<float> : 0.f;
    }

} // namespace filianore