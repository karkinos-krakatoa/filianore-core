#include "../fresnel/schlickdielectric.h"

#include <algorithm>
#include <iostream>

namespace filianore {

PrincipalSpectrum SchlickDielectric::evaluate(float cosThetaI) const {
    float R1 = 1.f - cosThetaI;
    float R = Ro + ((R1 * R1) * (R1 * R1) * R1) * (1.f - Ro);
    return PrincipalSpectrum(R);
}

} // namespace filianore