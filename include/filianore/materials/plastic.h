#ifndef _MATERIAL_PLASTIC_H
#define _MATERIAL_PLASTIC_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class PlasticMaterial : public Material
    {
    public:
        PlasticMaterial(
            const std::shared_ptr<Texture<PrincipalSpectrum>> _kd,
            const std::shared_ptr<Texture<float>> _diffRoughness,
            float _diffWeight,
            const std::shared_ptr<Texture<PrincipalSpectrum>> _ks,
            const std::shared_ptr<Texture<float>> _specRoughness,
            float _specWeight)
            : kd(_kd), diffRoughness(_diffRoughness), diffWeight(_diffWeight),
              ks(_ks), specRoughness(_specRoughness), specWeight(_specWeight)
        {
        }

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        std::shared_ptr<Texture<PrincipalSpectrum>> kd;
        std::shared_ptr<Texture<float>> diffRoughness;
        const float diffWeight;

        std::shared_ptr<Texture<PrincipalSpectrum>> ks;
        std::shared_ptr<Texture<float>> specRoughness;
        const float specWeight;
    };

} // namespace filianore

#endif