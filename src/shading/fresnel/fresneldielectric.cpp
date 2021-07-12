#include "filianore/shading/fresnel/fresneldielectric.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum FresnelDielectric::Evaluate(float cosI) const
    {
        cosI = Clamp<float>(cosI, -1, 1);
        // Potentially swap indices of refraction
        bool entering = cosI > 0.f;
        if (!entering)
        {
            std::swap(etaI, etaT);
            cosI = std::abs(cosI);
        }

        // Compute _cosThetaT_ using Snell's law
        float sinThetaI = std::sqrt(std::max((float)0, 1 - cosI * cosI));
        float sinThetaT = etaI / etaT * sinThetaI;

        // Handle total internal reflection
        if (sinThetaT >= 1)
            return 1;
        float cosThetaT = std::sqrt(std::max((float)0, 1 - sinThetaT * sinThetaT));
        float Rparl = ((etaT * cosI) - (etaI * cosThetaT)) /
                      ((etaT * cosI) + (etaI * cosThetaT));
        float Rperp = ((etaI * cosI) - (etaT * cosThetaT)) /
                      ((etaI * cosI) + (etaT * cosThetaT));
        return PrincipalSpectrum((Rparl * Rparl + Rperp * Rperp) / 2);
    }

} // namespace filianore