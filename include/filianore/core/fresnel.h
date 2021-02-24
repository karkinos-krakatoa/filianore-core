#ifndef _FRESNEL_H
#define _FRESNEL_H

#include "../maths/static_array.h"
#include "../color/principalspectrum.h"

namespace filianore
{

    class Fresnel
    {
    public:
        virtual ~Fresnel() {}

        virtual PrincipalSpectrum Evaluate(float cosI) const = 0;
    };

} // namespace filianore

#endif