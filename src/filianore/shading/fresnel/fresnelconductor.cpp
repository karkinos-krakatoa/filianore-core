#include "fresnelconductor.h"
#include "../../color/basespectrum.h"
#include "../../maths/mathutils.h"
#include <algorithm>
#include <iostream>

namespace filianore {

PrincipalSpectrum FresnelConductor::evaluate(float cosI) const {
    cosI = std::abs(cosI);

    cosI = filianore::clamp(cosI, -1.f, 1.f);
    PrincipalSpectrum eta = etaT / etaI;
    PrincipalSpectrum etak = k / etaI;

    float cosThetaI2 = cosI * cosI;
    float sinThetaI2 = 1. - cosThetaI2;
    PrincipalSpectrum eta2 = eta * eta;
    PrincipalSpectrum etak2 = etak * etak;

    PrincipalSpectrum t0 = eta2 - etak2 - sinThetaI2;
    PrincipalSpectrum a2plusb2 = (t0 * t0 + 4 * eta2 * etak2).spectral_sqrt();
    PrincipalSpectrum t1 = a2plusb2 + cosThetaI2;
    PrincipalSpectrum a = (0.5f * (a2plusb2 + t0)).spectral_sqrt();
    PrincipalSpectrum t2 = (float)2 * cosI * a;
    PrincipalSpectrum Rs = (t1 - t2) / (t1 + t2);

    PrincipalSpectrum t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
    PrincipalSpectrum t4 = t2 * sinThetaI2;
    PrincipalSpectrum Rp = Rs * (t3 - t4) / (t3 + t4);

    return 0.5 * (Rp + Rs);
}

} // namespace filianore