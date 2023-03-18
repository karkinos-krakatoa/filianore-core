#include "../fresnel/schlickmetallic.h"

#include <algorithm>
#include <iostream>

namespace filianore {

PrincipalSpectrum SchlickMetallic::evaluate(float cosThetaI) const {
    float r1 = 1.f - cosThetaI;
    return ro + (PrincipalSpectrum(1.f) - ro) * (r1 * r1 * r1 * r1 * r1);
}

} // namespace filianore