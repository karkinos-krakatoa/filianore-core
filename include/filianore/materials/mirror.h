#ifndef _MIRROR_MATERIAL_H
#define _MIRROR_MATERIAL_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class MirrorMaterial : public Material
    {
    public:
        MirrorMaterial(const std::shared_ptr<Texture<PrincipalSpectrum>> _kr)
            : kr(_kr) {}

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

    private:
        std::shared_ptr<Texture<PrincipalSpectrum>> kr;
    };

} // namespace filianore

#endif