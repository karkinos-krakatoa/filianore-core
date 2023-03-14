#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/core/shadingcore.h"

namespace filianore {

OrenNayarBRDF::OrenNayarBRDF(const PrincipalSpectrum &_R, float _weight, float sigma)
    : R(_R), weight(_weight), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)) {
    sigma = radians<float>(sigma);

    float sigma2 = sigma * sigma;
    A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
    B = 0.45f * sigma2 / (sigma2 + 0.09f);
}

PrincipalSpectrum OrenNayarBRDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    float sinThetaI = sin_theta(wi);
    float sinThetaO = sin_theta(wo);

    float maxCos = 0.f;
    if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
        float sinPhiI = sin_phi(wi);
        float cosPhiI = cos_phi(wi);

        float sinPhiO = sin_phi(wo);
        float cosPhiO = cos_phi(wo);

        float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
        maxCos = std::max(0.f, dCos);
    }

    float sinAlpha = 0.f;
    float tanBeta = 0.f;
    if (abs_cos_theta(wi) > abs_cos_theta(wo)) {
        sinAlpha = sinThetaO;
        tanBeta = sinThetaI / abs_cos_theta(wi);
    } else {
        sinAlpha = sinThetaI;
        tanBeta = sinThetaO / abs_cos_theta(wo);
    }

    return R * INV_PI * (A + B * maxCos * sinAlpha * tanBeta) * weight;
}

PrincipalSpectrum OrenNayarBRDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    *wi = cosine_hemisphere_sample(sample);

    if (wo.z < 0) {
        wi->z *= -1.f;
    }

    *_pdf = pdf(wo, *wi);
    *sampledType = this->bxDFType;

    return evaluate(wo, *wi);
}

float OrenNayarBRDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    return same_hemisphere(wo, wi) ? abs_cos_theta(wi) * INV_PI : 0.f;
}

} // namespace filianore