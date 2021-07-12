#ifndef _STANDARD_SURFACE_MATERIAL_H
#define _STANDARD_SURFACE_MATERIAL_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class StandardSurfaceMaterial : public Material
    {
    public:
        StandardSurfaceMaterial(const float _kdweight,
                                const std::shared_ptr<Texture<PrincipalSpectrum>> _kd,
                                const std::shared_ptr<Texture<float>> _kdroughness,

                                const float _ksweight,
                                const std::shared_ptr<Texture<PrincipalSpectrum>> _ks,
                                const std::shared_ptr<Texture<float>> _ksroughness,
                                const float _ksanisotropic)
            : kd(_kd), kdroughness(_kdroughness), kdweight(_kdweight),
              ks(_ks), ksroughness(_ksroughness), ksweight(_ksweight), ksanisotropic(_ksanisotropic)
        {
        }

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        // Diffuse Foundation
        std::shared_ptr<Texture<PrincipalSpectrum>> kd;
        std::shared_ptr<Texture<float>> kdroughness;
        float kdweight;

        // Specular
        std::shared_ptr<Texture<PrincipalSpectrum>> ks;
        std::shared_ptr<Texture<float>> ksroughness;
        float ksweight;
        float ksanisotropic;
    };

} // namespace filianore

#endif