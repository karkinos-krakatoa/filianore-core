#include "filianore/materials/lambert.h"
#include "filianore/materials/lambert.h"
#include "filianore/core/texture.h"
#include "filianore/core/interaction.h"
#include "filianore/core/memory.h"
#include "filianore/core/bsdf.h"

namespace filianore
{

    void LambertMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect, MemoryArena &arena) const
    {
        isect->bsdf = ARENA_ALLOCA(arena, BSDF)(*isect);
    }

} // namespace filianore