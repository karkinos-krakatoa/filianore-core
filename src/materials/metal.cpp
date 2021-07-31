#include "filianore/materials/metal.h"
#include "filianore/shading/fresnel/fresnelconductor.h"
#include "filianore/shading/microfacets/ggx.h"
#include "filianore/shading/bxdfs/microfacetreflection.h"
#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"
#include "filianore/color/metallurgy.h"
#include "filianore/color/spectrumoperations.h"

namespace filianore
{

    void MetalMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);

        PrincipalSpectrum ksSpectrum = ks->Evaluate(*isect);
        ksSpectrum = ksSpectrum.SpectrumClamp();

        float ksevalrough = ksroughness->Evaluate(*isect);

        float aspect = std::sqrt(1.f - ksanisotropic * 0.9f);
        float ax = std::max(.001f, (ksevalrough * ksevalrough) / aspect);
        float ay = std::max(.001f, (ksevalrough * ksevalrough) * aspect);

        std::shared_ptr<MicrofacetDistribution> distribution = std::make_shared<GGXDistribution>(ax, ay);

        PrincipalSpectrum eta = FromSPDExact(Gold_IOR);
        PrincipalSpectrum k = FromSPDExact(Gold_K);
        std::shared_ptr<Fresnel> fresnel = std::make_shared<FresnelConductor>(PrincipalSpectrum(1.f), eta, k);

        std::unique_ptr<BxDF> metallicBRDF = std::make_unique<MicrofacetReflectionBRDF>(distribution, fresnel, ksSpectrum);
        isect->bsdf->Add(metallicBRDF);
    }

} // namespace filianore
