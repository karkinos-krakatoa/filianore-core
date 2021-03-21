#ifndef _MATTE_MATERIAL_H
#define _MATTE_MATERIAL_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class MatteMaterial : public Material
    {
    public:
        MatteMaterial(const std::shared_ptr<Texture<PrincipalSpectrum>> _kd, const std::shared_ptr<Texture<float>> _roughness,
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