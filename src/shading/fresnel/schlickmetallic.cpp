#include "filianore/shading/fresnel/schlickmetallic.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum SchlickMetallic::Evaluate(float cosThetaI) const
    {
        float r1 = 1.f - cosThetaI;
        return ro + (PrincipalSpectrum(1.f) - ro) * (r1 * r1 * r1 * r1 * r1);
    }

} // namespace filianore