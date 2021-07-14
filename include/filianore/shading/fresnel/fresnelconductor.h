#ifndef _FRESNEL_CONDUCTOR_H
#define _FRESNEL_CONDUCTOR_H

#include "../../core/fresnel.h"

namespace filianore
{

    class FresnelConductor : public Fresnel
    {
    public:
        FresnelConductor(const PrincipalSpectrum &_eta, const PrincipalSpectrum &_k)
            : eta(_eta), k(_k)
        {
        }

        PrincipalSpectrum Evaluate(float cosI) const;

    private:
        PrincipalSpectrum eta, k;
    };

} // namespace filianore

#endif