#include "filianore/materials/standardsurface.h"

#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/shading/bxdfs/lambert.h"

#include "filianore/shading/fresnel/schlickdielectric.h"

#include "filianore/shading/microfacets/beckmann.h"
#include "filianore/shading/microfacets/ggx.h"

#include "filianore/shading/closures/fresnelblendeddiffspec.h"

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

        if (kdweight > 0 || ksweight > 0)
        {
            if (ksweight <= 0)
            {
                // Pure Diffuse
                if (kdevalrough == 0)
                {
                    std::unique_ptr<BxDF> lambBRDF = std::make_unique<LambertBRDF>(kdSpec, kdweight);
                    isect->bsdf->Add(lambBRDF);
                }
                else
                {
                    // Use OrenNayar and Remap [0-1] roughness to [0-90] sigma
                    float sigma = kdevalrough * 90.f;
                    std::unique_ptr<BxDF> orenBrdf = std::make_unique<OrenNayarBRDF>(kdSpec, kdweight, sigma);
                    isect->bsdf->Add(orenBrdf);
                }
            }
            else
            {
                // Specular : Blend with Diffuse using Fresnel Modulation
                float ksevalrough = ksroughness->Evaluate(*isect);
                float ksAnisotropic = ksanisotropic->Evaluate(*isect);

                float aspect = std::sqrt(1.f - ksAnisotropic * 0.9f);
                float ax = std::max(.001f, (ksevalrough * ksevalrough) / aspect);
                float ay = std::max(.001f, (ksevalrough * ksevalrough) * aspect);

                float ro = (1.f - ksIOR) / (1.f + ksIOR);
                ro = ro * ro;

                std::shared_ptr<Fresnel> dielectricFresnel = std::make_shared<SchlickDielectric>(ro);
                std::shared_ptr<MicrofacetDistribution> distribution = std::make_shared<GGXDistribution>(ax, ay);

                std::unique_ptr<BxDF> fresnelBlend = std::make_unique<FresnelBlendedDiffuseSpecularBRDF>(kdSpec, kdweight, kdevalrough,
                                                                                                         ksSpec, ksweight, ro, dielectricFresnel, distribution);
                isect->bsdf->Add(fresnelBlend);
            }
        }
    }

} // namespace filianore
