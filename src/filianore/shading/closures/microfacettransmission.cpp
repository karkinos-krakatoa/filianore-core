#include "../closures/microfacettransmission.h"
#include "../../core/microfacet.h"
#include "../../core/shadingcore.h"
#include "../fresnel/fresneldielectric.h"

namespace filianore {

MicrofacetTransmissionBRDF::MicrofacetTransmissionBRDF(const PrincipalSpectrum &_kr, float _krweight,
                                                       const PrincipalSpectrum &_kt, float _ktweight,
                                                       float _etaA, float _etaB,
                                                       const std::shared_ptr<MicrofacetDistribution> &_distribution)
    : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_GLOSSY)),
      kr(_kr), krweight(_krweight),
      kt(_kt), ktweight(_ktweight),
      etaA(_etaA), etaB(_etaB),
      distribution(_distribution) {
    fresnel = std::make_shared<FresnelDielectric>(etaA, etaB);
}

PrincipalSpectrum MicrofacetTransmissionBRDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    // float cosThetaO = cos_theta(wo), cosThetaI = cos_theta(wi);
    // if (cosThetaO == 0 || cosThetaI == 0)
    // {
    //     return PrincipalSpectrum(0.f);
    // }

    // bool reflect = cosThetaI * cosThetaO > 0;

    // float eta = 1.f, etaI = 1.f, etaT = 1.f;
    // if (!reflect)
    // {
    //     bool entering = cos_theta(wo) > 0;
    //     etaI = entering ? etaA : etaB;
    //     etaT = entering ? etaB : etaA;
    //     eta = etaI / etaT;
    // }

    // Vector3f wh = wi * eta + wo;
    // if (wh.length_squared() == 0)
    // {
    //     return PrincipalSpectrum(0.f);
    // }

    // wh = face_forward(normalize(wh), Vector3f(0.f, 0.f, 1.f));
    // if (wh.z < 0)
    //     wh = wh.Neg();

    // float F = fresnel->evaluate(dot(wo, wh)).c[0];

    // if (same_hemisphere(wo, wi))
    // {
    //     return kr * krweight * distribution->evaluate_distribution(wh) * distribution->evaluate_geometric(wo, wi) * F / std::abs(4.f * cosThetaO * cosThetaI);
    // }
    // else
    // {
    //     if (dot(wi, wh) * dot(wo, wh) > 0)
    //     {
    //         return PrincipalSpectrum(0.f);
    //     }

    //     float denom = dot(wi, wh) * eta + dot(wo, wh);
    //     denom *= denom;

    //     float factor = (etaI * etaI) / (etaT * etaT);

    //     return kt * ktweight * distribution->evaluate_distribution(wh) * distribution->evaluate_geometric(wo, wi) * (1.f - F) * factor * dot(wi, wh) * dot(wo, wh) /
    //            (cos_theta(wi) * cos_theta(wo) * denom);
    // }

    if (same_hemisphere(wo, wi))
        return 0; // transmission only

    float cosThetaO = cos_theta(wo);
    float cosThetaI = cos_theta(wi);
    if (cosThetaI == 0 || cosThetaO == 0)
        return PrincipalSpectrum(0);

    // Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
    float eta = cos_theta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
    Vector3f wh = normalize(wo + wi * eta);
    if (wh.z < 0)
        wh = -wh;

    // Same side?
    if (dot(wo, wh) * dot(wi, wh) > 0)
        return PrincipalSpectrum(0);

    PrincipalSpectrum F = fresnel->evaluate(dot(wo, wh));

    float sqrtDenom = dot(wo, wh) + eta * dot(wi, wh);
    float factor = (1 / eta);

    return (PrincipalSpectrum(1.f) - F) * kt *
           std::abs(distribution->evaluate_distribution(wh) * distribution->evaluate_geometric(wo, wi) * eta * eta *
                    abs_dot(wi, wh) * abs_dot(wo, wh) * factor * factor /
                    (cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));
}

PrincipalSpectrum MicrofacetTransmissionBRDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    // Vector3f wh = distribution->sample_wh(wo, sample);
    // float F = fresnel->evaluate(dot(wo, wh)).c[0];

    // if (sample.x < F)
    // {
    //     *wi = Reflect(wo, wh);

    //     if (!same_hemisphere(wo, *wi)) // Reflection Only
    //     {
    //         return PrincipalSpectrum(0.f);
    //     }

    //     *pdf = distribution->pdf(wo, wh) * F / (4.f * abs_dot(wo, wh));
    //     if (sampledType)
    //     {
    //         *sampledType = BxDFType(BSDF_GLOSSY | BSDF_REFLECTION);
    //     }

    //     float cosThetaO = cos_theta(wo), cosThetaI = cos_theta(*wi);
    //     if (cosThetaO == 0 || cosThetaI == 0)
    //     {
    //         return PrincipalSpectrum(0.f);
    //     }

    //     return kr * krweight * distribution->evaluate_distribution(wh) * distribution->evaluate_geometric(wo, *wi) * F / (4.f * cosThetaO * cosThetaI);
    // }
    // else
    // {
    //     bool entering = cos_theta(wo) > 0;
    //     float etaI = entering ? etaA : etaB;
    //     float etaT = entering ? etaB : etaA;
    //     float eta = etaI / etaT;
    //     if (!Refract(wo, wh, eta, wi))
    //     {
    //         return PrincipalSpectrum(0.f);
    //     }

    //     if (same_hemisphere(wo, *wi)) // Transmission Only
    //     {
    //         return PrincipalSpectrum(0.f);
    //     }

    //     if (wi->z == 0)
    //     {
    //         return PrincipalSpectrum(0.f);
    //     }

    //     float denom = dot(*wi, wh) * etaI + dot(wo, wh) * etaT;
    //     denom *= denom;

    //     float factor = (etaI * etaI) / (etaT * etaT);

    //     float dwh_dwi = abs_dot(*wi, wh) / denom;
    //     *pdf = distribution->pdf(wo, wh) * (1.f - F) * dwh_dwi;

    //     if (sampledType)
    //     {
    //         *sampledType = BxDFType(BSDF_GLOSSY | BSDF_TRANSMISSION);
    //     }

    //     return kt * ktweight * distribution->evaluate_distribution(wh) * distribution->evaluate_geometric(wo, *wi) * (1.f - F) * factor * dot(*wi, wh) * dot(wo, wh) /
    //            (cos_theta(*wi) * cos_theta(wo) * denom);
    // }

    if (wo.z == 0)
        return 0.;

    Vector3f wh = distribution->sample_wh(wo, sample);
    if (dot(wo, wh) < 0)
        return 0.; // Should be rare

    float eta = cos_theta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);
    if (!refract(wo, wh, eta, wi))
        return 0;
    *_pdf = pdf(wo, *wi);
    return evaluate(wo, *wi);
}

float MicrofacetTransmissionBRDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    // float cosThetaO = cos_theta(wo), cosThetaI = cos_theta(wi);
    // if (cosThetaO == 0 || cosThetaI == 0)
    // {
    //     return 0.f;
    // }

    // bool reflect = cosThetaI * cosThetaO > 0;

    // float eta = 1.f, etaI = 1.f, etaT = 1.f;
    // if (!reflect)
    // {
    //     bool entering = cos_theta(wo) > 0;
    //     etaI = entering ? etaA : etaB;
    //     etaT = entering ? etaB : etaA;
    //     eta = etaI / etaT;
    // }

    // Vector3f wh = wi * eta + wo;
    // if (wh.length_squared() == 0)
    // {
    //     return 0.f;
    // }

    // wh = face_forward(normalize(wh), Vector3f(0.f, 0.f, 1.f));
    // if (wh.z < 0)
    //     wh = wh.Neg();

    // float F = fresnel->evaluate(dot(wo, wh)).c[0];
    // if (same_hemisphere(wo, wi))
    // {
    //     return distribution->pdf(wo, wh) * F / (4.f * abs_dot(wo, wh));
    // }
    // else
    // {
    //     if (dot(wi, wh) * dot(wo, wh) > 0)
    //     {
    //         return 0.f;
    //     }

    //     float denom = dot(wi, wh) * etaI + dot(wo, wh) * etaT;
    //     denom *= denom;

    //     float dwh_dwi = abs_dot(wi, wh) / denom;
    //     return distribution->pdf(wo, wh) * (1.f - F) * dwh_dwi;
    // }

    if (same_hemisphere(wo, wi))
        return 0;
    // Compute $\wh$ from $\wo$ and $\wi$ for microfacet transmission
    float eta = cos_theta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
    Vector3f wh = normalize(wo + wi * eta);

    if (dot(wo, wh) * dot(wi, wh) > 0)
        return 0;

    // Compute change of variables _dwh\_dwi_ for microfacet transmission
    float sqrtDenom = dot(wo, wh) + eta * dot(wi, wh);
    float dwh_dwi =
        std::abs((eta * eta * dot(wi, wh)) / (sqrtDenom * sqrtDenom));
    return distribution->pdf(wo, wh) * dwh_dwi;
}
} // namespace filianore