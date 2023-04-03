#ifndef _FRESNEL_H
#define _FRESNEL_H

#include "../../color/principalspectrum.h"
#include "../../maths/mathutils.h"

namespace filianore {

inline float sp_amp_reflection_coefficient(float n1, float n2, float cosI, float cosT) {
    return (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
}

inline float sp_amp_transmission_coefficient(float n1, float n2, float cosI, float cosT) {
    return (2.f * n1 * cosI) / (n1 * cosI + n2 * cosT);
}

inline float pp_amp_reflection_coefficient(float n1, float n2, float cosI, float cosT) {
    return (n2 * cosI - n1 * cosT) / (n1 * cosT + n2 * cosI);
}

inline float pp_amp_transmission_coefficient(float n1, float n2, float cosI, float cosT) {
    return (2.f * n1 * cosI) / (n1 * cosT + n2 * cosI);
}

inline float fresnel_dielectric_evaluation(float cosThetaI, float etaI, float etaT) {
    cosThetaI = clamp<float>(cosThetaI, -1, 1);

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
    if (sinThetaT >= 1) {
        return 1.f;
    }

    float cosThetaT = std::sqrt(std::max(0.f, 1 - sinThetaT * sinThetaT));

    // R_parallel
    float Rparl = (etaT * cosThetaI - etaI * cosThetaT) /
                  (etaT * cosThetaI + etaI * cosThetaT);
    // R_perpendicular
    float Rperp = (etaI * cosThetaI - etaT * cosThetaT) /
                  (etaI * cosThetaI + etaT * cosThetaT);
    // Fr
    float Fr = (Rparl * Rparl + Rperp * Rperp) * 0.5;

    return Fr;
}

class Fresnel {
public:
    virtual ~Fresnel() {}

    virtual PrincipalSpectrum evaluate(float cosI) const = 0;
};

} // namespace filianore

#endif