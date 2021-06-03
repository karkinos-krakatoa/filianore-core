#include "filianore/materials/plastic.h"
#include "filianore/shading/orennayarbrdf.h"
#include "filianore/shading/fresneldielectric.h"
#include "filianore/shading/trowbridgereitzdistribution.h"
#include "filianore/shading/beckmann.h"
#include "filianore/shading/microfacetreflection.h"
#include "filianore/shading/specularreflection.h"
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

        std::shared_ptr<Fresnel> fresnel = std::make_shared<FresnelDielectric>(1.52f, 1.f);
        float specRough = specRoughness->Evaluate(*isect);

        float rough = BeckmannDistribution::RoughnessToAlpha(0.f);

        std::shared_ptr<MicrofacetDistribution> distrib = std::make_unique<BeckmannDistribution>(rough, rough);

        std::unique_ptr<BxDF> microfacetRefl = std::make_unique<MicrofacetReflection>(ksSpectrum, distrib, fresnel);
        isect->bsdf->Add(microfacetRefl);
    }

} // namespace filianore
