#include "filianore/shading/fresnel/schlickdielectric.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum SchlickDielectric::Evaluate(float cosThetaI) const
    {
        float R1 = 1.f - cosThetaI;
        float R = Ro + ((R1 * R1) * (R1 * R1) * R1) * (1.f - Ro);
        return PrincipalSpectrum(R);
    }

} // namespace filianore