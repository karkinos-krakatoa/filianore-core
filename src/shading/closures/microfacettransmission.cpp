#include "filianore/shading/closures/microfacettransmission.h"
#include "filianore/core/microfacet.h"
#include "filianore/shading/fresnel/fresneldielectric.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    MicrofacetTransmissionBRDF::MicrofacetTransmissionBRDF(const PrincipalSpectrum &_kr, float _krweight,
                                                           const PrincipalSpectrum &_kt, float _ktweight,
                                                           float _etaA, float _etaB,
                                                           const std::shared_ptr<MicrofacetDistribution> &_distribution)
        : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_GLOSSY)),
          kr(_kr), krweight(_krweight),
          kt(_kt), ktweight(_ktweight),
          etaA(_etaA), etaB(_etaB),
          distribution(_distribution)
    {
        fresnel = std::make_shared<FresnelDielectric>(etaA, etaB);
    }

    PrincipalSpectrum MicrofacetTransmissionBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        // float cosThetaO = CosTheta(wo), cosThetaI = CosTheta(wi);
        // if (cosThetaO == 0 || cosThetaI == 0)
        // {
        //     return PrincipalSpectrum(0.f);
        // }

        // bool reflect = cosThetaI * cosThetaO > 0;

        // float eta = 1.f, etaI = 1.f, etaT = 1.f;
        // if (!reflect)
        // {
        //     bool entering = CosTheta(wo) > 0;
        //     etaI = entering ? etaA : etaB;
        //     etaT = entering ? etaB : etaA;
        //     eta = etaI / etaT;
        // }

        // StaticArray<float, 3> wh = wi * eta + wo;
        // if (wh.LengthSquared() == 0)
        // {
        //     return PrincipalSpectrum(0.f);
        // }

        // wh = Faceforward(wh.Normalize(), StaticArray<float, 3>(0.f, 0.f, 1.f));
        // if (wh.z() < 0)
        //     wh = wh.Neg();

        // float F = fresnel->Evaluate(Dot(wo, wh)).c[0];

        // if (SameHemisphere(wo, wi))
        // {
        //     return kr * krweight * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) * F / std::abs(4.f * cosThetaO * cosThetaI);
        // }
        // else
        // {
        //     if (Dot(wi, wh) * Dot(wo, wh) > 0)
        //     {
        //         return PrincipalSpectrum(0.f);
        //     }

        //     float denom = Dot(wi, wh) * eta + Dot(wo, wh);
        //     denom *= denom;

        //     float factor = (etaI * etaI) / (etaT * etaT);

        //     return kt * ktweight * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) * (1.f - F) * factor * Dot(wi, wh) * Dot(wo, wh) /
        //            (CosTheta(wi) * CosTheta(wo) * denom);
        // }

        if (SameHemisphere(wo, wi))
            return 0; // transmission only

        float cosThetaO = CosTheta(wo);
        float cosThetaI = CosTheta(wi);
        if (cosThetaI == 0 || cosThetaO == 0)
            return PrincipalSpectrum(0);

        // Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
        float eta = CosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
        StaticArray<float, 3> wh = (wo + wi * eta).Normalize();
        if (wh.z() < 0)
            wh = wh.Neg();

        // Same side?
        if (Dot(wo, wh) * Dot(wi, wh) > 0)
            return PrincipalSpectrum(0);

        PrincipalSpectrum F = fresnel->Evaluate(Dot(wo, wh));

        float sqrtDenom = Dot(wo, wh) + eta * Dot(wi, wh);
        float factor = (1 / eta);

        return (PrincipalSpectrum(1.f) - F) * kt *
               std::abs(distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) * eta * eta *
                        AbsDot(wi, wh) * AbsDot(wo, wh) * factor * factor /
                        (cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));
    }

    PrincipalSpectrum MicrofacetTransmissionBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        // StaticArray<float, 3> wh = distribution->SampleWh(wo, sample);
        // float F = fresnel->Evaluate(Dot(wo, wh)).c[0];

        // if (sample.x() < F)
        // {
        //     *wi = Reflect(wo, wh);

        //     if (!SameHemisphere(wo, *wi)) // Reflection Only
        //     {
        //         return PrincipalSpectrum(0.f);
        //     }

        //     *pdf = distribution->Pdf(wo, wh) * F / (4.f * AbsDot(wo, wh));
        //     if (sampledType)
        //     {
        //         *sampledType = BxDFType(BSDF_GLOSSY | BSDF_REFLECTION);
        //     }

        //     float cosThetaO = CosTheta(wo), cosThetaI = CosTheta(*wi);
        //     if (cosThetaO == 0 || cosThetaI == 0)
        //     {
        //         return PrincipalSpectrum(0.f);
        //     }

        //     return kr * krweight * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, *wi) * F / (4.f * cosThetaO * cosThetaI);
        // }
        // else
        // {
        //     bool entering = CosTheta(wo) > 0;
        //     float etaI = entering ? etaA : etaB;
        //     float etaT = entering ? etaB : etaA;
        //     float eta = etaI / etaT;
        //     if (!Refract(wo, wh, eta, wi))
        //     {
        //         return PrincipalSpectrum(0.f);
        //     }

        //     if (SameHemisphere(wo, *wi)) // Transmission Only
        //     {
        //         return PrincipalSpectrum(0.f);
        //     }

        //     if (wi->params[2] == 0)
        //     {
        //         return PrincipalSpectrum(0.f);
        //     }

        //     float denom = Dot(*wi, wh) * etaI + Dot(wo, wh) * etaT;
        //     denom *= denom;

        //     float factor = (etaI * etaI) / (etaT * etaT);

        //     float dwh_dwi = AbsDot(*wi, wh) / denom;
        //     *pdf = distribution->Pdf(wo, wh) * (1.f - F) * dwh_dwi;

        //     if (sampledType)
        //     {
        //         *sampledType = BxDFType(BSDF_GLOSSY | BSDF_TRANSMISSION);
        //     }

        //     return kt * ktweight * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, *wi) * (1.f - F) * factor * Dot(*wi, wh) * Dot(wo, wh) /
        //            (CosTheta(*wi) * CosTheta(wo) * denom);
        // }

        if (wo.z() == 0)
            return 0.;

        StaticArray<float, 3> wh = distribution->SampleWh(wo, sample);
        if (Dot(wo, wh) < 0)
            return 0.; // Should be rare

        float eta = CosTheta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);
        if (!Refract(wo, wh, eta, wi))
            return 0;
        *pdf = Pdf(wo, *wi);
        return Evaluate(wo, *wi);
    }

    float MicrofacetTransmissionBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        // float cosThetaO = CosTheta(wo), cosThetaI = CosTheta(wi);
        // if (cosThetaO == 0 || cosThetaI == 0)
        // {
        //     return 0.f;
        // }

        // bool reflect = cosThetaI * cosThetaO > 0;

        // float eta = 1.f, etaI = 1.f, etaT = 1.f;
        // if (!reflect)
        // {
        //     bool entering = CosTheta(wo) > 0;
        //     etaI = entering ? etaA : etaB;
        //     etaT = entering ? etaB : etaA;
        //     eta = etaI / etaT;
        // }

        // StaticArray<float, 3> wh = wi * eta + wo;
        // if (wh.LengthSquared() == 0)
        // {
        //     return 0.f;
        // }

        // wh = Faceforward(wh.Normalize(), StaticArray<float, 3>(0.f, 0.f, 1.f));
        // if (wh.z() < 0)
        //     wh = wh.Neg();

        // float F = fresnel->Evaluate(Dot(wo, wh)).c[0];
        // if (SameHemisphere(wo, wi))
        // {
        //     return distribution->Pdf(wo, wh) * F / (4.f * AbsDot(wo, wh));
        // }
        // else
        // {
        //     if (Dot(wi, wh) * Dot(wo, wh) > 0)
        //     {
        //         return 0.f;
        //     }

        //     float denom = Dot(wi, wh) * etaI + Dot(wo, wh) * etaT;
        //     denom *= denom;

        //     float dwh_dwi = AbsDot(wi, wh) / denom;
        //     return distribution->Pdf(wo, wh) * (1.f - F) * dwh_dwi;
        // }

        if (SameHemisphere(wo, wi))
            return 0;
        // Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
        float eta = CosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
        StaticArray<float, 3> wh = (wo + wi * eta).Normalize();

        if (Dot(wo, wh) * Dot(wi, wh) > 0)
            return 0;

        // Compute change of variables _dwh\_dwi_ for microfacet transmission
        float sqrtDenom = Dot(wo, wh) + eta * Dot(wi, wh);
        float dwh_dwi =
            std::abs((eta * eta * Dot(wi, wh)) / (sqrtDenom * sqrtDenom));
        return distribution->Pdf(wo, wh) * dwh_dwi;
    }
}