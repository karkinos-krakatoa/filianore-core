#ifndef _MATERIAL_METAL_H
#define _MATERIAL_METAL_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class MetalMaterial : public Material
    {
    public:
        MetalMaterial(
            const float _ksweight,
            const std::shared_ptr<Texture<PrincipalSpectrum>> _ks,
            const std::shared_ptr<Texture<float>> _ksroughness,
            const float _ksanisotropic)
            : ks(_ks), ksroughness(_ksroughness), ksweight(_ksweight), ksanisotropic(_ksanisotropic)
        {
        }

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        std::shared_ptr<Texture<PrincipalSpectrum>> ks;
        std::shared_ptr<Texture<float>> ksroughness;
        float ksweight;
        mutable float ksanisotropic;
    };

} // namespace filianore

#endif