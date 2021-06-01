#include "filianore/shading/fresneldielectric.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum FresnelDielectric::Evaluate(float cosI) const
    {
        float cosThetaI = Clamp<float>(cosI, -1.f, 1.f);

        // Swap indices of refraction, if necessary
        bool entering = cosThetaI > 0.f;
        if (!entering)
        {
            std::swap(etaI, etaT);
            cosThetaI = std::abs(cosThetaI);
        }

        // Snell's law
        float sinThetaI = std::sqrt(std::max(0.f, 1.f - cosThetaI * cosThetaI));
        float sinThetaT = etaI / etaT * sinThetaI;

        // Total internal reflection
        if (sinThetaT >= 1)
        {
            return 1.f;
        }

        float cosThetaT = std::sqrt(std::max(0.f, 1.f - sinThetaT * sinThetaT));
        float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                      ((etaT * cosThetaI) + (etaI * cosThetaT));
        float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                      ((etaI * cosThetaI) + (etaT * cosThetaT));

        return (Rparl * Rparl + Rperp * Rperp) * 0.5f;
    }

} // namespace filianore