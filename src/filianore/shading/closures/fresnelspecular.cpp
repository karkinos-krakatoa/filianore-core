#include "../closures/fresnelspecular.h"
#include "../../core/microfacet.h"
#include "../../core/shadingcore.h"
#include "../fresnel/fresneldielectric.h"

namespace filianore {

FresnelSpecularBXDF::FresnelSpecularBXDF(const PrincipalSpectrum &_R, const PrincipalSpectrum &_T, float _etaA, float _etaB)
    : R(_R), T(_T), etaA(_etaA), etaB(_etaB),
      BxDF(BxDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)) {
    fresnel = std::make_shared<FresnelDielectric>(etaA, etaB);
}

PrincipalSpectrum FresnelSpecularBXDF::evaluate(const Vector3f &wo, const Vector3f &wi) const {
    return PrincipalSpectrum(0.f);
}

PrincipalSpectrum FresnelSpecularBXDF::sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *_pdf, BxDFType *sampledType) const {
    float F = fresnel->evaluate(cos_theta(wo)).c[0];

    if (sample.x < F) {
        *wi = Vector3f(-wo.x, -wo.y, wo.z);
        if (sampledType) {
            *sampledType = BxDFType(BSDF_SPECULAR | BSDF_REFLECTION);
        }

        *_pdf = F;
        return R * F / abs_cos_theta(*wi);
    } else {
        bool entering = cos_theta(wo) > 0;
        float etaI = entering ? etaA : etaB;
        float etaT = entering ? etaB : etaA;

        // Compute ray direction for specular transmission
        if (!refract(wo, face_forward(Vector3f(0.f, 0.f, 1.f), wo), etaI / etaT, wi)) {
            return 0.f;
        }
        PrincipalSpectrum ft = T * (1.f - F);

        ft *= (etaI * etaI) / (etaT * etaT);

        if (sampledType) {
            *sampledType = BxDFType(BSDF_SPECULAR | BSDF_TRANSMISSION);
        }

        *_pdf = 1.f - F;
        return ft / abs_cos_theta(*wi);
    }
}

float FresnelSpecularBXDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
    return 0.f;
}

} // namespace filianore