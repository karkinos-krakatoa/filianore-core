#include "filianore/shading/fresnel/fresnelnull.h"

namespace filianore {

PrincipalSpectrum FresnelNull::evaluate(float cosI) const {
    return PrincipalSpectrum(1.f);
}

} // namespace filianore