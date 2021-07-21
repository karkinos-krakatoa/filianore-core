#include "filianore/materials/plastic.h"
#include "filianore/shading/bxdfs/fresnelblend.h"
#include "filianore/shading/microfacets/ggx.h"
#include "filianore/shading/bxdfs/microfacetreflection.h"
#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void PlasticMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {

        isect->bsdf = std::make_shared<BSDF>(*isect);

        PrincipalSpectrum kdSpectrum = kd->Evaluate(*isect);
        kdSpectrum = kdSpectrum.SpectrumClamp();

        PrincipalSpectrum ksSpectrum = ks->Evaluate(*isect);
        ksSpectrum = ksSpectrum.SpectrumClamp();

        std::shared_ptr<MicrofacetDistribution> distribution = std::make_shared<GGXDistribution>(0.01f, 0.01f);

        std::unique_ptr<BxDF> fresnelBlend = std::make_unique<FresnelBlend>(kdSpectrum, ksSpectrum, distribution);
        isect->bsdf->Add(fresnelBlend);
    }

} // namespace filianore
