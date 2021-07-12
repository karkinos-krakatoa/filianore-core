#include "filianore/shading/bxdfs/microfacetdielectricreflection.h"
#include "filianore/shading/microfacets/ggx.h"
#include "filianore/shading/microfacets/beckmann.h"
#include "filianore/shading/fresnel/fresneldielectric.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    MicrofacetDielectricReflectionBRDF::MicrofacetDielectricReflectionBRDF(const PrincipalSpectrum &_R, const float alphax, const float alphay)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(_R)
    {
        fresnel = std::make_shared<FresnelDielectric>(1.52f, 1.f);
        distribution = std::make_shared<BeckmannDistribution>(alphax, alphay);
    }

    PrincipalSpectrum MicrofacetDielectricReflectionBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
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

        return R * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) * F / (4.f * cosThetaO * cosThetaI);
    }

    PrincipalSpectrum MicrofacetDielectricReflectionBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
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

        *pdf = distribution->Pdf(wo, wh) / (4 * Dot(wo, wh));

        return Evaluate(wo, *wi);
    }

    float MicrofacetDielectricReflectionBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (!SameHemisphere(wo, wi))
        {
            return 0.f;
        }
        StaticArray<float, 3> wh = (wo + wi).Normalize();
        return distribution->Pdf(wo, wh) / (4.f * Dot(wo, wh));
    }

}