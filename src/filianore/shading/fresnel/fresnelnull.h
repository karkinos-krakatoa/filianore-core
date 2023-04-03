#ifndef _FRESNEL_NULL_H
#define _FRESNEL_NULL_H

#include "fresnel.h"

namespace filianore {

class FresnelNull : public Fresnel {
public:
    FresnelNull() {}

    PrincipalSpectrum evaluate(float cosI) const;
};

} // namespace filianore

#endif