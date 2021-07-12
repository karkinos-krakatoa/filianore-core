#ifndef _STANDARD_SURFACE_MATERIAL_H
#define _STANDARD_SURFACE_MATERIAL_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class StandardSurfaceMaterial : public Material
    {
    public:
        StandardSurfaceMaterial(const std::shared_ptr<Texture<PrincipalSpectrum>> _kd,
                                const std::shared_ptr<Texture<float>> _roughness,
                                float _weight)
            : kd(_kd), roughness(_roughness), weight(_weight) {}

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        std::shared_ptr<Texture<PrincipalSpectrum>> kd;
        std::shared_ptr<Texture<float>> roughness;
        float weight;
    };

} // namespace filianore

#endif