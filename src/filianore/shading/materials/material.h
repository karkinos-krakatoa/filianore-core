#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "../../core/elemental.h"
#include <memory>

namespace filianore {

class Material {
public:
    virtual ~Material() {}

    virtual void compute_scattering_functions(SurfaceInteraction *isect) const = 0;

    static void bump_evaluate(const std::shared_ptr<Texture<float>> &bumpMap, SurfaceInteraction *isect);
};

} // namespace filianore

#endif