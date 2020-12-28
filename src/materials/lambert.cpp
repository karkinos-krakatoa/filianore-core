#include "filianore/materials/lambert.h"
#include "filianore/materials/lambert.h"
#include "filianore/core/texture.h"
#include "filianore/core/interaction.h"
#include "filianore/core/memory.h"
#include "filianore/core/bsdf.h"

namespace filianore
{

    void LambertMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);
        Color r = kd->Evaluate(*isect);
        isect->bsdf->Add(new LambertBxDF(r));
    }

} // namespace filianore
