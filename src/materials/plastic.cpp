#include "filianore/materials/plastic.h"
#include "filianore/shading/bxdfs/plastic.h"
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

        float ksevalrough = ksroughness->Evaluate(*isect);

        float aspect = std::sqrt(1.f - ksanisotropic * 0.9f);
        float ax = std::max(.001f, (ksevalrough * ksevalrough) / aspect);
        float ay = std::max(.001f, (ksevalrough * ksevalrough) * aspect);

        std::shared_ptr<MicrofacetDistribution> distribution = std::make_shared<GGXDistribution>(ax, ay);

        float ro = (1.f - ksIOR) / (1.f + ksIOR);
        ro = ro * ro;

        std::unique_ptr<BxDF> fresnelBlend = std::make_unique<PlasticBRDF>(kdweight, kdSpectrum, ksweight, ksSpectrum, ro, distribution);
        isect->bsdf->Add(fresnelBlend);
    }

} // namespace filianore
