#ifndef _FRESNEL_H
#define _FRESNEL_H

#include "../maths/static_array.h"
#include "../color/rgb.h"

namespace filianore
{

    class Fresnel
    {
    public:
        virtual ~Fresnel() {}

        virtual RGBSpectrum Evaluate(float cosI) const = 0;
    };

} // namespace filianore

#endif