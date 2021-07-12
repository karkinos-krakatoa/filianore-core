#include "filianore/materials/standardsurface.h"

#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/shading/bxdfs/lambert.h"

#include "filianore/shading/fresnel/fresneldielectric.h"

#include "filianore/shading/microfacets/beckmann.h"
#include "filianore/shading/bxdfs/microfacetreflection.h"

#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    void StandardSurfaceMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        isect->bsdf = std::make_shared<BSDF>(*isect);

        // Diffuse
        if (kdweight > 0)
        {
            PrincipalSpectrum kdSpectrum = kd->Evaluate(*isect);
            kdSpectrum = kdSpectrum.SpectrumClamp() * kdweight;
            float kdevalrough = kdroughness->Evaluate(*isect);

            if (kdevalrough == 0)
            {
                std::unique_ptr<BxDF> lambBRDF = std::make_unique<LambertBRDF>(kdSpectrum);
                isect->bsdf->Add(lambBRDF);
            }
            else
            {
                // Remap [0-1] roughness to [0-90] sigma
                float sigma = kdevalrough * 90.f;
                std::unique_ptr<BxDF> orenBrdf = std::make_unique<OrenNayarBRDF>(kdSpectrum, sigma);
                isect->bsdf->Add(orenBrdf);
            }
        }

        // Specular
        if (ksweight > 0)
        {
            PrincipalSpectrum ksSpectrum = ks->Evaluate(*isect);
            ksSpectrum = ksSpectrum.SpectrumClamp() * ksweight;
            float ksevalrough = ksroughness->Evaluate(*isect);

            ksevalrough = ksevalrough * ksevalrough;
            float alpha = BeckmannDistribution::RoughnessToAlpha(ksevalrough);
            float aspect = std::sqrt(1 - ksanisotropic * .9);
            float alphax = std::max(.001f, (alpha / aspect));
            float alphay = std::max(.001f, (alpha * aspect));

            std::shared_ptr<FresnelDielectric> fresnel = std::make_shared<FresnelDielectric>(1.52f, 1.f);
            std::shared_ptr<BeckmannDistribution> distribution = std::make_shared<BeckmannDistribution>(alphax, alphay);

            std::unique_ptr<BxDF> microfacetRefl = std::make_unique<MicrofacetReflectionBRDF>(distribution, fresnel, ksSpectrum, alphax, alphay);
            isect->bsdf->Add(microfacetRefl);
        }
    }

} // namespace filianore
