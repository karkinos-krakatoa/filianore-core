#ifndef _FRESNEL_CONDUCTOR_H
#define _FRESNEL_CONDUCTOR_H

#include "../../core/fresnel.h"

namespace filianore {

class FresnelConductor : public Fresnel {
public:
    FresnelConductor(const PrincipalSpectrum &_etaI, const PrincipalSpectrum &_etaT, const PrincipalSpectrum &_k)
        : etaI(_etaI), etaT(_etaT), k(_k) {
    }

    PrincipalSpectrum evaluate(float cosI) const;

private:
    PrincipalSpectrum etaI, etaT, k;
};

} // namespace filianore

#endif