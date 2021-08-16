#include "filianore/shading/closures/microfacettransmission.h"
#include "filianore/core/microfacet.h"
#include "filianore/shading/fresnel/fresneldielectric.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    MicrofacetTransmissionBRDF::MicrofacetTransmissionBRDF(const std::shared_ptr<MicrofacetDistribution> &_distribution,
                                                           const float _etaA, const float _etaB, const PrincipalSpectrum &_T, float _weight)
        : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_GLOSSY)), T(_T), distribution(_distribution), etaA(_etaA), etaB(_etaB), weight(_weight)
    {
        fresnel = std::make_shared<FresnelDielectric>(etaA, etaB);
    }

    PrincipalSpectrum MicrofacetTransmissionBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (CosTheta(wo) == 0 || CosTheta(wi) == 0)
            return PrincipalSpectrum(0.0f);

        if (SameHemisphere(wo, wi))
        {
            auto wh = (wo + wi).Normalize();
            if (wh.z() < 0)
                wh = wh.Neg();
            auto d = distribution->EvaluateD(wh);
            auto g = distribution->EvaluateG(wo, wi);
            auto f = fresnel->Evaluate(Dot(wo, wh)).c[0];
            return PrincipalSpectrum(1.f) * d * g * f / (4 * CosTheta(wo) * CosTheta(wi));
        }

        float eta = CosTheta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);
        auto wh = (wo + wi * eta).Normalize();
        if (wh.z() < 0)
            wh = wh.Neg();
        auto cosThetaH = Dot(wo, wh);
        auto f = fresnel->Evaluate(Dot(wo, wh)).c[0];
        if (f == 1.0f)
            return PrincipalSpectrum(0.0f);
        auto cosThetaT = Dot(wi, wh);
        auto sqrtDenom = cosThetaT + cosThetaH / eta;
        return T * ((1.0f - f) * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) *
                    std::abs(cosThetaH * cosThetaT / (CosTheta(wo) * CosTheta(wi) * sqrtDenom * sqrtDenom)));
    }

    PrincipalSpectrum MicrofacetTransmissionBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        StaticArray<float, 3> wh = distribution->SampleWh(wo, sample);
        float F = fresnel->Evaluate(Dot(wo, wh)).c[0];

        if (sample.x() < F)
        {
            *wi = Reflect(wo, wh);

            if (!SameHemisphere(wo, *wi))
            {
                return PrincipalSpectrum(0.f);
            }

            *pdf = distribution->Pdf(wo, wh) * F / (4.f * Dot(wo, wh));

            float cosTheta_o = AbsCosTheta(wo), cosTheta_i = AbsCosTheta(*wi);

            return PrincipalSpectrum(1.f) * weight * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, *wi) * F / (4.f * cosTheta_o * cosTheta_i);
        }
        else
        {
            float eta = CosTheta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);
            if (!Refract(wo, wh, eta, wi))
                return PrincipalSpectrum(0.f);

            if (SameHemisphere(wo, *wi))
            {
                return PrincipalSpectrum(0.f);
            }

            float denom1 = Dot(wo, wh) + eta * Dot(*wi, wh);
            float denom = denom1 * denom1;
            float factor = (1 / eta) * (1 / eta);

            float dwm_dwi = AbsDot(*wi, wh) / denom;
            *pdf = distribution->Pdf(wo, wh) * (1.f - F) * dwm_dwi;

            return T * factor *
                   std::abs(distribution->EvaluateD(wh) * distribution->EvaluateG(wo, *wi) *
                            Dot(*wi, wh) * Dot(wo, wh) /
                            (CosTheta(*wi) * CosTheta(wo) * denom));
        }
    }

    float MicrofacetTransmissionBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (SameHemisphere(wo, wi))
        {
            auto wh = (wo + wi).Normalize();
            if (wh.z() < 0)
                wh = wh.Neg();
            auto cosThetaH = Dot(wo, wh);
            auto f = fresnel->Evaluate(cosThetaH).c[0];
            return f * distribution->Pdf(wo, wi) / (4.0f * std::abs(cosThetaH));
        }

        float eta = CosTheta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);
        auto wh = (wo + wi * eta).Normalize();
        if (wh.z() < 0)
            wh = wh.Neg();
        auto cosThetaH = Dot(wo, wh);
        auto f = fresnel->Evaluate(cosThetaH).c[0];
        auto cosThetaT = Dot(wi, wh);
        auto sqrtDenom = cosThetaT + cosThetaH / eta;
        return (1.0f - f) * distribution->Pdf(wo, wi) * std::abs(cosThetaT) / (sqrtDenom * sqrtDenom);
    }

}