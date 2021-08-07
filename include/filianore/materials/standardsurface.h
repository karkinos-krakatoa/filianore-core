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

                                const float _metallicWeight,

                                const float _ksweight,
                                const std::shared_ptr<Texture<PrincipalSpectrum>> _ks,
                                const std::shared_ptr<Texture<float>> _ksroughness,
                                const std::shared_ptr<Texture<float>> _ksanisotropic,
                                const float _ksIOR,

                                const float _sheenweight,
                                const std::shared_ptr<Texture<PrincipalSpectrum>> _sheenColor,
                                const std::shared_ptr<Texture<float>> _sheenroughness)
            : kd(_kd), kdroughness(_kdroughness), kdweight(_kdweight),
              metallicWeight(_metallicWeight),
              ks(_ks), ksroughness(_ksroughness), ksweight(_ksweight), ksanisotropic(_ksanisotropic), ksIOR(_ksIOR),
              sheenweight(_sheenweight), sheenColor(_sheenColor), sheenroughness(_sheenroughness)
        {
        }

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        // Diffuse Foundation
        std::shared_ptr<Texture<PrincipalSpectrum>> kd;
        std::shared_ptr<Texture<float>> kdroughness;
        float kdweight;

        // Metallic
        float metallicWeight;

        // Specular
        std::shared_ptr<Texture<PrincipalSpectrum>> ks;
        std::shared_ptr<Texture<float>> ksroughness;
        std::shared_ptr<Texture<float>> ksanisotropic;
        float ksweight;
        float ksIOR;

        // Sheen
        std::shared_ptr<Texture<PrincipalSpectrum>> sheenColor;
        std::shared_ptr<Texture<float>> sheenroughness;
        float sheenweight;
    };

} // namespace filianore

#endif