#ifndef _SCHLICK_DIELECTRIC_H
#define _SCHLICK_DIELECTRIC_H

#include "../../core/fresnel.h"

namespace filianore
{

    class SchlickDielectric : public Fresnel
    {
    public:
        SchlickDielectric(float etaI, float etaT)
        {
            float ro = std::abs(etaI - etaT) / (etaI + etaT);
            Ro = ro * ro;
        }

        PrincipalSpectrum Evaluate(float cosThetaI) const;

    private:
        float Ro;
    };

} // namespace filianore

#endif