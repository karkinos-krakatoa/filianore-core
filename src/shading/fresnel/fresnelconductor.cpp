#include "filianore/shading/fresnel/fresnelconductor.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum FresnelConductor::Evaluate(float cosI) const
    {
        cosI = fabs(cosI);
        PrincipalSpectrum tmp = (eta * eta + k * k) * cosI * cosI;
        PrincipalSpectrum Rparl2 = (tmp - (2.f * eta * cosI) + 1) /
                                   (tmp + (2.f * eta * cosI) + 1);
        PrincipalSpectrum tmp_f = eta * eta + k * k;
        PrincipalSpectrum Rperp2 =
            (tmp_f - (2.f * eta * cosI) + cosI * cosI) /
            (tmp_f + (2.f * eta * cosI) + cosI * cosI);
        return (Rparl2 + Rperp2) / 2.f;
    }

} // namespace filianore