#include "filianore/materials/standardsurface.h"

#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/shading/bxdfs/lambert.h"

#include "filianore/color/metallurgy.h"
#include "filianore/color/spectrumoperations.h"

#include "filianore/shading/fresnel/fresnelconductor.h"
#include "filianore/shading/fresnel/schlickdielectric.h"
#include "filianore/shading/fresnel/fresnelnull.h"

#include "filianore/shading/microfacets/estevez.h"
#include "filianore/shading/microfacets/ggx.h"

#include "filianore/shading/closures/fresnelblendeddiffspec.h"
#include "filianore/shading/closures/microfacetreflection.h"

#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void StandardSurfaceMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);

        // Prepare Parameters
        PrincipalSpectrum kdSpec = kd->Evaluate(*isect).SpectrumClamp();
        PrincipalSpectrum ksSpec = ks->Evaluate(*isect).SpectrumClamp();
        float kdevalrough = kdroughness->Evaluate(*isect);

        if (ksweight <= 0 || metallicWeight > 0)
        {
            // Pure Diffuse
            if (kdevalrough == 0)
            {
                std::unique_ptr<BxDF> lambBRDF = std::make_unique<LambertBRDF>(kdSpec, kdweight);
                isect->bsdf->Add(lambBRDF);
            }
            else
            {
                // Remap [0-1] roughness to [0-90] sigma
                float sigma = kdevalrough * 90.f;
                std::unique_ptr<BxDF> orenBrdf = std::make_unique<OrenNayarBRDF>(kdSpec, kdweight, sigma);
                isect->bsdf->Add(orenBrdf);
            }
        }

        float ksevalrough = ksroughness->Evaluate(*isect);
        float ksAnisotropic = ksanisotropic->Evaluate(*isect);

        float aspect = std::sqrt(1.f - ksAnisotropic * 0.9f);
        float ax = std::max(.001f, (ksevalrough * ksevalrough) / aspect);
        float ay = std::max(.001f, (ksevalrough * ksevalrough) * aspect);

        std::shared_ptr<MicrofacetDistribution> distribution = std::make_shared<GGXDistribution>(ax, ay);
        if (metallicWeight > 0)
        {
            std::pair<const float *, const float *> metalPair = GetMetalNameFromValue(0);
            PrincipalSpectrum eta = FromSPDExact(metalPair.first);
            PrincipalSpectrum k = FromSPDExact(metalPair.second);
            std::shared_ptr<Fresnel> fresnel = std::make_shared<FresnelConductor>(PrincipalSpectrum(1.f), eta, k);
            std::unique_ptr<BxDF> metallicBRDF = std::make_unique<MicrofacetReflectionBRDF>(distribution, fresnel, PrincipalSpectrum(1.f), metallicWeight);
            isect->bsdf->Add(metallicBRDF);
        }

        if (ksweight > 0 && metallicWeight <= 0)
        {
            // Specular : Blend with Diffuse using Fresnel Modulation
            float ro = (1.f - ksIOR) / (1.f + ksIOR);
            ro = ro * ro;

            std::shared_ptr<Fresnel> dielectricFresnel = std::make_shared<SchlickDielectric>(ro);
            std::unique_ptr<BxDF> specularBrdf = std::make_unique<FresnelBlendedDiffuseSpecularBRDF>(kdSpec, kdweight, kdevalrough,
                                                                                                     ksSpec, ksweight, ro, dielectricFresnel, distribution);
            isect->bsdf->Add(specularBrdf);
        }

        if (sheenweight > 0)
        {
            PrincipalSpectrum sheenCol = sheenColor->Evaluate(*isect).SpectrumClamp();
            float sheenevalrough = sheenroughness->Evaluate(*isect);
            std::shared_ptr<Fresnel> sheenFresnel = std::make_shared<FresnelNull>();
            std::shared_ptr<MicrofacetDistribution> sheenDistribution = std::make_shared<EstevezSheenDistribution>(sheenevalrough);
            std::unique_ptr<BxDF> sheenBRDF = std::make_unique<MicrofacetReflectionBRDF>(sheenDistribution, sheenFresnel, sheenCol, sheenweight);
            isect->bsdf->Add(sheenBRDF);
        }
    }

} // namespace filianore
