#include "filianore/materials/mirror.h"
#include "filianore/shading/specularreflection.h"
#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);

        RGBSpectrum r = kr->Evaluate(*isect);
        r = r.SpecClamp();

        isect->bsdf->Add(new SpecularReflection(r));
    }

} // namespace filianore
