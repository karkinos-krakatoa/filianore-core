#include "filianore/materials/lambert.h"
#include "filianore/shading/bxdfs/lambert.h"
#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void LambertMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);

        PrincipalSpectrum r = kd->Evaluate(*isect);
        r = r.SpectrumClamp();

        std::unique_ptr<BxDF> lambRefl = std::make_unique<LambertBRDF>(r, 1.f);
        isect->bsdf->Add(lambRefl);
    }

} // namespace filianore
