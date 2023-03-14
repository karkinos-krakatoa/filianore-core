#include "filianore/shading/bxdfs/lambert.h"
#include "filianore/core/shadingcore.h"

namespace filianore {

PrincipalSpectrum LambertBRDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    return R * INV_PI * weight;
}

PrincipalSpectrum LambertBRDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    *wi = cosine_hemisphere_sample(sample);

    if (wo.z < 0) {
        wi->z *= -1.f;
    }

    *_pdf = pdf(wo, *wi);
    *sampledType = this->bxDFType;

    return evaluate(wo, *wi);
}

float LambertBRDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    return same_hemisphere(wo, wi) ? abs_cos_theta(wi) * INV_PI : 0.f;
}

} // namespace filianore