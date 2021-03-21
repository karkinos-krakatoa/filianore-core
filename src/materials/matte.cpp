#include "filianore/materials/matte.h"
#include "filianore/shading/orennayarbrdf.h"
#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void MatteMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);

        PrincipalSpectrum r = kd->Evaluate(*isect);
        r = r.SpectrumClamp() * weight;

        float rough = roughness->Evaluate(*isect);

        // Remap [0-1] roughness to [0-90] sigma
        float sigma = rough * 90.f;

        std::unique_ptr<BxDF> orenBrdf = std::make_unique<OrenNayarBRDF>(r, sigma);
        isect->bsdf->Add(orenBrdf);
    }

} // namespace filianore
