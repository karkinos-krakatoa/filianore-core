#ifndef _LAMBERT_MATERIAL_H
#define _LAMBERT_MATERIAL_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class LambertMaterial : public Material
    {
    public:
        LambertMaterial(const std::shared_ptr<Texture<RGBSpectrum>> _kd)
            : kd(_kd) {}

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        std::shared_ptr<Texture<RGBSpectrum>> kd;
    };

} // namespace filianore

#endif