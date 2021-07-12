#ifndef _FRESNEL_NULL_H
#define _FRESNEL_NULL_H

#include "../../core/fresnel.h"

namespace filianore
{

    class FresnelNull : public Fresnel
    {
    public:
        FresnelNull() {}

        PrincipalSpectrum Evaluate(float cosI) const;
    };

} // namespace filianore

#endif