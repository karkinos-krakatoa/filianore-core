#ifndef _SCHLICK_DIELECTRIC_H
#define _SCHLICK_DIELECTRIC_H

#include "../../core/fresnel.h"

namespace filianore
{

    class SchlickDielectric : public Fresnel
    {
    public:
        SchlickDielectric(float _Ro)
            : Ro(_Ro)
        {
        }

        PrincipalSpectrum Evaluate(float cosThetaI) const;

    private:
        const float Ro;
    };

} // namespace filianore

#endif