#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../../core/elemental.h"
#include "../../maths/geometry.h"

namespace filianore {

template <typename T>
class Texture {
public:
    virtual ~Texture() {}
    virtual T evaluate(const SurfaceInteraction &isect) const = 0;
};

} // namespace filianore

#endif