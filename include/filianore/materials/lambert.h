#ifndef _LAMBERT_H
#define _LAMBERT_H

#include "../core/elemental.h"
#include "../core/material.h"

namespace filianore
{

    class LambertMaterial : public Material
    {
    public:
        LambertMaterial(const std::shared_ptr<Texture<Color>> _kd)
            : kd(_kd) {}

        void ComputeScatteringFunctions(SurfaceInteraction *isect, MemoryArena &arena) const;

    private:
        std::shared_ptr<Texture<Color>> kd;
    };

} // namespace filianore

#endif