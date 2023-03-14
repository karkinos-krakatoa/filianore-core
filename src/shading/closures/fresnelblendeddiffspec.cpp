#include "filianore/shading/closures/fresnelblendeddiffspec.h"
#include "filianore/core/microfacet.h"
#include "filianore/core/shadingcore.h"
#include "filianore/shading/fresnel/thinfilm.h"

namespace filianore {

FresnelBlendedDiffuseSpecularBRDF::FresnelBlendedDiffuseSpecularBRDF(const PrincipalSpectrum &_kd, float _kdWeight, float _kdRoughness,

                                                                     const PrincipalSpectrum &_ks, float _ksWeight, float _ro,

                                                                     const std::shared_ptr<Fresnel> &_fresnel,
                                                                     const std::shared_ptr<MicrofacetDistribution> &_distribution)
    : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)),
      kd(_kd), kdWeight(_kdWeight), kdRoughness(_kdRoughness),
      ks(_ks), ksWeight(_ksWeight), ro(_ro),
      fresnel(_fresnel), distribution(_distribution) {
    orenNayarBrdf = std::make_shared<OrenNayarBRDF>(kd, kdWeight, kdRoughness);
}

PrincipalSpectrum FresnelBlendedDiffuseSpecularBRDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    // Diffuse
    float r1 = 1.f - .5f * abs_cos_theta(wi);
    float r1pow5 = (r1 * r1) * (r1 * r1) * r1;
    float r2 = 1.f - .5f * abs_cos_theta(wo);
    float r2pow5 = (r2 * r2) * (r2 * r2) * r2;

    PrincipalSpectrum diffuse = (28.f / 23.f) * orenNayarBrdf->evaluate(wo, wi) * (1.f - ro) * (1 - r1pow5) * (1 - r2pow5);

    // Specular
    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) {
        return PrincipalSpectrum(0);
    }
    wh = normalize(wh);

    PrincipalSpectrum specular = distribution->evaluate_distribution(wh) * (fresnel->evaluate(dot(wi, wh))) / (4.f * abs_dot(wi, wh) * std::max(abs_cos_theta(wi), abs_cos_theta(wo)));
    specular = ks * specular * ksWeight;

    return diffuse + specular;
}

PrincipalSpectrum FresnelBlendedDiffuseSpecularBRDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    Vector2f u = sample;
    if (u.x < .5) {
        u[0] = std::min(2 * u.x, 1.f - SHADOW_EPSILON);
        *wi = cosine_hemisphere_sample(u);
        if (wo.z < 0) {
            wi->z *= -1.f;
        }
    } else {
        u[0] = std::min(2 * (u.x - .5f), 1.f - SHADOW_EPSILON);
        Vector3f wh = distribution->sample_wh(wo, u);
        *wi = reflect(wo, wh);
        if (!same_hemisphere(wo, *wi))
            return PrincipalSpectrum(0.f);
    }
    *_pdf = pdf(wo, *wi);
    return evaluate(wo, *wi);
}

float FresnelBlendedDiffuseSpecularBRDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    if (!same_hemisphere(wo, wi))
        return 0;
    Vector3f wh = normalize(wo + wi);
    float pdf_wh = distribution->pdf(wo, wh);
    return .5f * (abs_cos_theta(wi) * INV_PI + pdf_wh / (4 * dot(wo, wh)));
}

} // namespace filianore