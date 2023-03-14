#include "filianore/shading/bxdfs/diffusetransmission.h"
#include "filianore/core/shadingcore.h"
#include "filianore/shading/bxdfs/orennayar.h"

namespace filianore {

DiffuseTransmission::DiffuseTransmission(const PrincipalSpectrum &_T, float _weight, float sigma)
    : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_DIFFUSE)) {
    diffuseBxdf = std::make_unique<OrenNayarBRDF>(_T, _weight, sigma);
}

PrincipalSpectrum DiffuseTransmission::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    return diffuseBxdf->evaluate(wo, wi);
}

PrincipalSpectrum DiffuseTransmission::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    *wi = cosine_hemisphere_sample(sample);

    if (wo.z > 0) {
        wi->z *= -1.f;
    }

    *_pdf = pdf(wo, *wi);
    *sampledType = this->bxDFType;

    return evaluate(wo, *wi);
}

float DiffuseTransmission::pdf(const Vector3f &wo, const Vector3f &wi) const {
    return !same_hemisphere(wo, wi) ? abs_cos_theta(wi) * INV_PI : 0.f;
}

} // namespace filianore