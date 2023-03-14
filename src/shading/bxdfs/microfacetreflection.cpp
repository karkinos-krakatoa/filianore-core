#include "filianore/shading/bxdfs/microfacetreflection.h"
#include "filianore/core/fresnel.h"
#include "filianore/core/microfacet.h"
#include "filianore/core/shadingcore.h"

namespace filianore {

MicrofacetReflectionBRDF::MicrofacetReflectionBRDF(const std::shared_ptr<MicrofacetDistribution> &_distribution,
                                                   const std::shared_ptr<Fresnel> &_fresnel, const PrincipalSpectrum &_R, float _weight)
    : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(_R), distribution(_distribution), fresnel(_fresnel), weight(_weight) {
}

PrincipalSpectrum MicrofacetReflectionBRDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    float cosThetaO = abs_cos_theta(wo);
    float cosThetaI = abs_cos_theta(wi);

    Vector3f wh = wi + wo;

    if (cosThetaI == 0 || cosThetaO == 0) {
        return PrincipalSpectrum(0.f);
    }

    if (wh.x == 0 && wh.y == 0 && wh.z == 0) {
        return PrincipalSpectrum(0.f);
    }

    wh = normalize(wh);

    PrincipalSpectrum F = fresnel->evaluate(dot(wi, face_forward(wh, Vector3f(0.f, 0.f, 1.f))));

    return R * weight * distribution->evaluate_distribution(wh) * distribution->evaluate_geometric(wo, wi) * F / (4.f * cosThetaO * cosThetaI);
}

PrincipalSpectrum MicrofacetReflectionBRDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    if (wo.z == 0) {
        return PrincipalSpectrum(0.f);
    }

    Vector3f wh = distribution->sample_wh(wo, sample);
    if (dot(wo, wh) < 0) {
        return PrincipalSpectrum(0.f);
    }

    *wi = reflect(wo, wh);

    if (!same_hemisphere(wo, *wi)) {
        return PrincipalSpectrum(0.f);
    }

    *_pdf = distribution->pdf(wo, wh) / (4.f * dot(wo, wh));

    return evaluate(wo, *wi);
}

float MicrofacetReflectionBRDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    if (!same_hemisphere(wo, wi)) {
        return 0.f;
    }
    Vector3f wh = normalize(wo + wi);
    return distribution->pdf(wo, wh) / (4.f * dot(wo, wh));
}

} // namespace filianore