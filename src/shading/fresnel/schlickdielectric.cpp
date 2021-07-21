#include "filianore/shading/fresnel/schlickdielectric.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum SchlickDielectric::Evaluate(float cosThetaI) const
    {
        auto pow5 = [](float v)
        { return (v * v) * (v * v) * v; };
        float R = Ro + pow5(1 - cosThetaI) * (1.f - Ro);
        return PrincipalSpectrum(R);
    }

} // namespace filianore