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

        PrincipalSpectrum r = kr->Evaluate(*isect);
        r = r.SpectrumClamp();

        std::unique_ptr<BxDF> specRefl = std::make_unique<SpecularReflection>(r);
        isect->bsdf->Add(specRefl);
    }

} // namespace filianore
