#ifndef _LAMBERT_MATERIAL_H
#define _LAMBERT_MATERIAL_H

#include "../../core/elemental.h"
#include "material.h"

namespace filianore {

class LambertMaterial : public Material {
public:
    LambertMaterial(const std::shared_ptr<Texture<PrincipalSpectrum>> _kd)
        : kd(_kd) {}

    void compute_scattering_functions(SurfaceInteraction *isect) const;

private:
    std::shared_ptr<Texture<PrincipalSpectrum>> kd;
};

} // namespace filianore

#endif