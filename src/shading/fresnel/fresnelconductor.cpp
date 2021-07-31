#include "filianore/shading/fresnel/fresnelconductor.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum FresnelConductor::Evaluate(float cosI) const
    {
        cosI = std::abs(cosI);

        cosI = Clamp<float>(cosI, -1.f, 1.f);
        PrincipalSpectrum eta = etaT / etaI;
        PrincipalSpectrum etak = k / etaI;

        float cosThetaI2 = cosI * cosI;
        float sinThetaI2 = 1. - cosThetaI2;
        PrincipalSpectrum eta2 = eta * eta;
        PrincipalSpectrum etak2 = etak * etak;

        PrincipalSpectrum t0 = eta2 - etak2 - sinThetaI2;
        PrincipalSpectrum a2plusb2 = Sqrt(t0 * t0 + 4 * eta2 * etak2);
        PrincipalSpectrum t1 = a2plusb2 + cosThetaI2;
        PrincipalSpectrum a = Sqrt(0.5f * (a2plusb2 + t0));
        PrincipalSpectrum t2 = (float)2 * cosI * a;
        PrincipalSpectrum Rs = (t1 - t2) / (t1 + t2);

        PrincipalSpectrum t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
        PrincipalSpectrum t4 = t2 * sinThetaI2;
        PrincipalSpectrum Rp = Rs * (t3 - t4) / (t3 + t4);

        return 0.5 * (Rp + Rs);
    }

} // namespace filianore