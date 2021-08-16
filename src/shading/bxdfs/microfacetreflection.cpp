#include "filianore/shading/bxdfs/microfacetreflection.h"
#include "filianore/core/microfacet.h"
#include "filianore/core/fresnel.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    MicrofacetReflectionBRDF::MicrofacetReflectionBRDF(const std::shared_ptr<MicrofacetDistribution> &_distribution,
                                                       const std::shared_ptr<Fresnel> &_fresnel, const PrincipalSpectrum &_R, float _weight)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(_R), distribution(_distribution), fresnel(_fresnel), weight(_weight)
    {
    }

    PrincipalSpectrum MicrofacetReflectionBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        float cosThetaO = AbsCosTheta(wo);
        float cosThetaI = AbsCosTheta(wi);

        StaticArray<float, 3> wh = wi + wo;

        if (cosThetaI == 0 || cosThetaO == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        if (wh.x() == 0 && wh.y() == 0 && wh.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        wh = wh.Normalize();

        PrincipalSpectrum F = fresnel->Evaluate(Dot(wi, Faceforward(wh, StaticArray<float, 3>(0.f, 0.f, 1.f))));

        return R * weight * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) * F / (4.f * cosThetaO * cosThetaI);
    }

    PrincipalSpectrum MicrofacetReflectionBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        if (wo.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        StaticArray<float, 3> wh = distribution->SampleWh(wo, sample);
        if (Dot(wo, wh) < 0)
        {
            return PrincipalSpectrum(0.f);
        }

        *wi = Reflect(wo, wh);

        if (!SameHemisphere(wo, *wi))
        {
            return PrincipalSpectrum(0.f);
        }

        *pdf = distribution->Pdf(wo, wh) / (4.f * Dot(wo, wh));

        return Evaluate(wo, *wi);
    }

    float MicrofacetReflectionBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (!SameHemisphere(wo, wi))
        {
            return 0.f;
        }
        StaticArray<float, 3> wh = (wo + wi).Normalize();
        return distribution->Pdf(wo, wh) / (4.f * Dot(wo, wh));
    }

}