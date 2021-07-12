#include "filianore/materials/plastic.h"
#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/shading/microfacets/ggx.h"
#include "filianore/shading/microfacets/beckmann.h"
#include "filianore/shading/bxdfs/microfacetdielectricreflection.h"
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

        std::unique_ptr<BxDF> microfacetRefl = std::make_unique<MicrofacetDielectricReflectionBRDF>(ksSpectrum, alphax, alphay);
        isect->bsdf->Add(microfacetRefl);
    }

} // namespace filianore
