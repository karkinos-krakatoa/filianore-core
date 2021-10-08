#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "elemental.h"
#include "../maths/static_array.h"

namespace filianore
{

    template <typename T>
    class Texture
    {
    public:
        virtual ~Texture() {}
        virtual T Evaluate(const SurfaceInteraction &isect) const = 0;
    };

} // namespace filianore

#endif