#ifndef _MATERIAL_PLASTIC_H
#define _MATERIAL_PLASTIC_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class PlasticMaterial : public Material
    {
    public:
        PlasticMaterial(const float _kdweight,
                        const std::shared_ptr<Texture<PrincipalSpectrum>> _kd,

                        const float _ksweight,
                        const std::shared_ptr<Texture<PrincipalSpectrum>> _ks,
                        const std::shared_ptr<Texture<float>> _ksroughness,
                        const float _ksanisotropic,
                        const float _ksIOR)
            : kd(_kd), kdweight(_kdweight),
              ks(_ks), ksroughness(_ksroughness), ksweight(_ksweight), ksanisotropic(_ksanisotropic), ksIOR(_ksIOR)
        {
        }

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        // Diffuse
        std::shared_ptr<Texture<PrincipalSpectrum>> kd;
        std::shared_ptr<Texture<float>> kdroughness;
        float kdweight;

        // Specular
        std::shared_ptr<Texture<PrincipalSpectrum>> ks;
        std::shared_ptr<Texture<float>> ksroughness;
        float ksweight;
        mutable float ksanisotropic;
        float ksIOR;
    };

} // namespace filianore

#endif