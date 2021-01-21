#include "filianore/materials/lambert.h"
#include "filianore/materials/lambert.h"
#include "filianore/shading/lambertreflection.h"
#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void LambertMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);
        Spectrum<float> r = kd->Evaluate(*isect);
        isect->bsdf->Add(new LambertReflection(r));
    }

} // namespace filianore
