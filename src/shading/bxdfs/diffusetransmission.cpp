#include "filianore/shading/bxdfs/diffusetransmission.h"
#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    DiffuseTransmission::DiffuseTransmission(const PrincipalSpectrum &_T, float _weight, float sigma)
        : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_DIFFUSE))
    {
        diffuseBxdf = std::make_unique<OrenNayarBRDF>(_T, _weight, sigma);
    }

    PrincipalSpectrum DiffuseTransmission::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return diffuseBxdf->Evaluate(wo, wi);
    }

    PrincipalSpectrum DiffuseTransmission::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        *wi = CosineHemisphereSample(sample);

        if (wo.z() > 0)
        {
            wi->params[2] *= -1.f;
        }

        *pdf = Pdf(wo, *wi);
        *sampledType = this->bxDFType;

        return Evaluate(wo, *wi);
    }

    float DiffuseTransmission::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return !SameHemisphere(wo, wi) ? AbsCosTheta(wi) * InvPi<float> : 0.f;
    }

} // namespace filianore