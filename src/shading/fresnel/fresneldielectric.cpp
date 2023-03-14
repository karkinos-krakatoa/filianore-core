#include "filianore/shading/fresnel/fresneldielectric.h"
#include "filianore/maths/mathutils.h"
#include <algorithm>
#include <iostream>

namespace filianore {

PrincipalSpectrum FresnelDielectric::evaluate(float cosThetaI) const {
    cosThetaI = filianore::clamp<float>(cosThetaI, -1.f, 1.f);

    // Check and swap the indices of refraction
    bool enter = cosThetaI > 0.0;
    if (!enter) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    // Compute sin and cos terms using Snell's law
    float sinThetaI = std::sqrt(std::max(0.f, 1.f - cosThetaI * cosThetaI));
    float sinThetaT = etaI / etaT * sinThetaI;

    // Handle total internal reflection
    if (sinThetaT >= 1)
        return PrincipalSpectrum(1.f);

    float cosThetaT = std::sqrt(std::max(0.f, 1 - sinThetaT * sinThetaT));

    // R_parallel
    float Rparl = (etaT * cosThetaI - etaI * cosThetaT) /
                  (etaT * cosThetaI + etaI * cosThetaT);
    // R_perpendicular
    float Rperp = (etaI * cosThetaI - etaT * cosThetaT) /
                  (etaI * cosThetaI + etaT * cosThetaT);
    // Fr
    float Fr = (Rparl * Rparl + Rperp * Rperp) * 0.5;

    return PrincipalSpectrum(Fr);
}

} // namespace filianore