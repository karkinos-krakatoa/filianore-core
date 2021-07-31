#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    OrenNayarBRDF::OrenNayarBRDF(const PrincipalSpectrum &_R, float _weight, float sigma)
        : R(_R), weight(_weight), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE))
    {
        sigma = Radians<float>(sigma);

        float sigma2 = sigma * sigma;
        A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
        B = 0.45f * sigma2 / (sigma2 + 0.09f);
    }

    PrincipalSpectrum OrenNayarBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        float sinThetaI = SinTheta(wi);
        float sinThetaO = SinTheta(wo);

        float maxCos = 0.f;
        if (sinThetaI > 1e-4 && sinThetaO > 1e-4)
        {
            float sinPhiI = SinPhi(wi);
            float cosPhiI = CosPhi(wi);

            float sinPhiO = SinPhi(wo);
            float cosPhiO = CosPhi(wo);

            float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
            maxCos = std::max(0.f, dCos);
        }

        float sinAlpha = 0.f;
        float tanBeta = 0.f;
        if (AbsCosTheta(wi) > AbsCosTheta(wo))
        {
            sinAlpha = sinThetaO;
            tanBeta = sinThetaI / AbsCosTheta(wi);
        }
        else
        {
            sinAlpha = sinThetaI;
            tanBeta = sinThetaO / AbsCosTheta(wo);
        }

        return R * InvPi<float> * (A + B * maxCos * sinAlpha * tanBeta) * weight;
    }

    PrincipalSpectrum OrenNayarBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
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

    float OrenNayarBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return SameHemisphere(wo, wi) ? AbsCosTheta(wi) * InvPi<float> : 0.f;
    }

} // namespace filianore