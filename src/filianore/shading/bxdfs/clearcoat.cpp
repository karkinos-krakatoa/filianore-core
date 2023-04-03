#include "clearcoat.h"
#include "../../core/shadingcore.h"
#include "../../maths/geometry.h"
#include "../fresnel/fresneldielectric.h"
#include "../microfacets/microfacet.h"

namespace filianore {
inline float GTR1(float cosTheta, float alpha) {
    float alpha2 = alpha * alpha;
    return (alpha2 - 1) /
           (PI * std::log(alpha2) * (1 + (alpha2 - 1) * cosTheta * cosTheta));
}

// Smith masking/shadowing term.
inline float smithG_GGX(float cosTheta, float alpha) {
    float alpha2 = alpha * alpha;
    float cosTheta2 = cosTheta * cosTheta;
    return 1.f / (cosTheta + sqrt(alpha2 + cosTheta2 - alpha2 * cosTheta2));
}

ClearcoatReflectionBRDF::ClearcoatReflectionBRDF(const PrincipalSpectrum &_R, float _ior, float _weight, float _gloss)
    : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(_R), ior(_ior), weight(_weight), gloss(_gloss) {
    fresnel = std::make_shared<FresnelDielectric>(1.f, ior);
    gloss = lerp<float>(gloss, .1f, .001f);
}

PrincipalSpectrum ClearcoatReflectionBRDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) {
        return PrincipalSpectrum(0.);
    }

    wh = normalize(wh);

    float Dr = GTR1(abs_cos_theta(wh), gloss);
    PrincipalSpectrum Fr = fresnel->evaluate(dot(wo, wh));

    // The geometric term always based on alpha = 0.25.
    float Gr = smithG_GGX(abs_cos_theta(wo), .25) * smithG_GGX(abs_cos_theta(wi), .25);

    return R * Fr * weight * Gr * Dr / 4;
}

PrincipalSpectrum ClearcoatReflectionBRDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    if (wo.z == 0) {
        return PrincipalSpectrum(0.f);
    }

    float alpha2 = gloss * gloss;
    float cosTheta = std::sqrt(std::max(0.f, (1 - std::pow(alpha2, 1 - sample.x)) / (1 - alpha2)));
    float sinTheta = std::sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
    float phi = 2 * PI * sample.y;

    Vector3f wh = spherical_direction<float>(sinTheta, cosTheta, phi);
    if (!same_hemisphere(wo, wh)) {
        wh = -wh;
    }

    *wi = reflect(wo, wh);
    if (!same_hemisphere(wo, *wi)) {
        return PrincipalSpectrum(0.f);
    }

    *_pdf = pdf(wo, *wi);
    return evaluate(wo, *wi);
}

float ClearcoatReflectionBRDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    if (!same_hemisphere(wo, wi))
        return 0;

    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0)
        return 0;

    wh = normalize(wh);

    float Dr = GTR1(abs_cos_theta(wh), gloss);
    return Dr * abs_cos_theta(wh) / (4 * dot(wo, wh));
}

} // namespace filianore