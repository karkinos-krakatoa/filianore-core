#include "filianore/materials/plastic.h"
#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/shading/microfacets/ggx.h"
#include "filianore/shading/microfacets/beckmann.h"
#include "filianore/shading/bxdfs/microfacetreflection.h"
#include "filianore/shading/fresnel/fresneldielectric.h"
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

        float sigma = diffRoughness->Evaluate(*isect) * 90.f;
        std::unique_ptr<BxDF> orenBrdf = std::make_unique<OrenNayarBRDF>(kdSpectrum, sigma);
        isect->bsdf->Add(orenBrdf);

        float anisotripic = 0.f;
        float roughness = BeckmannDistribution::RoughnessToAlpha(0.f);
        float aspect = std::sqrt(1 - anisotripic * .9);
        float alphax = std::max(.001f, (roughness / aspect));
        float alphay = std::max(.001f, (roughness * aspect));

        std::shared_ptr<FresnelDielectric> fresnel = std::make_shared<FresnelDielectric>(1.52f, 1.f);
        std::shared_ptr<BeckmannDistribution> distribution = std::make_shared<BeckmannDistribution>(alphax, alphay);

        std::unique_ptr<BxDF> microfacetRefl = std::make_unique<MicrofacetReflectionBRDF>(distribution, fresnel, ksSpectrum, alphax, alphay);
        isect->bsdf->Add(microfacetRefl);
    }

} // namespace filianore
