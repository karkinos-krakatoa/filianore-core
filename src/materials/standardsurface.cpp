#include "filianore/materials/standardsurface.h"

#include "filianore/textures/imagemap.h"

#include "filianore/shading/bxdfs/diffusetransmission.h"
#include "filianore/shading/bxdfs/clearcoat.h"
#include "filianore/shading/bxdfs/orennayar.h"
#include "filianore/shading/bxdfs/lambert.h"
#include "filianore/shading/bxdfs/microfacetreflection.h"

#include "filianore/color/metallurgy.h"
#include "filianore/color/spectrumoperations.h"

#include "filianore/shading/fresnel/fresnelconductor.h"
#include "filianore/shading/fresnel/schlickdielectric.h"
#include "filianore/shading/fresnel/schlickmetallic.h"
#include "filianore/shading/fresnel/fresnelnull.h"
#include "filianore/shading/fresnel/thinfilm.h"

#include "filianore/shading/microfacets/estevez.h"
#include "filianore/shading/microfacets/ggx.h"

#include "filianore/shading/closures/fresnelblendeddiffspec.h"
#include "filianore/shading/closures/microfacettransmission.h"
#include "filianore/shading/closures/fresnelspecular.h"

#include "filianore/core/interaction.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/texture.h"

namespace filianore
{

    StandardSurfaceMaterial::StandardSurfaceMaterial(const float _kdweight,
                                                     const std::shared_ptr<Texture<PrincipalSpectrum>> &_kd,
                                                     const std::shared_ptr<Texture<float>> &_kdroughness,

                                                     const float _ksweight,
                                                     const std::shared_ptr<Texture<PrincipalSpectrum>> &_ks,
                                                     const std::shared_ptr<Texture<float>> &_ksroughness,
                                                     const std::shared_ptr<Texture<float>> &_ksanisotropic,
                                                     const int _fresnelType,
                                                     const float _ksIOR,
                                                     const int _metalType,

                                                     const float _sheenweight,
                                                     const std::shared_ptr<Texture<PrincipalSpectrum>> &_sheenColor,
                                                     const std::shared_ptr<Texture<float>> &_sheenroughness,

                                                     const float _ktweight,
                                                     const std::shared_ptr<Texture<PrincipalSpectrum>> &_kt,

                                                     const float _krcoatweight,
                                                     const std::shared_ptr<Texture<PrincipalSpectrum>> &_krcoat,
                                                     const float _krcoatior,
                                                     const float _krcoatgloss,

                                                     const float _thinFilmThickness,
                                                     const float _thinFilmIOR,

                                                     const bool _thinWalled,
                                                     const std::shared_ptr<Texture<float>> &_bumpMap)
        : kd(_kd), kdroughness(_kdroughness), kdweight(_kdweight),
          ks(_ks), ksroughness(_ksroughness), ksweight(_ksweight), ksanisotropic(_ksanisotropic), fresnelType(_fresnelType), ksIOR(_ksIOR), metalType(_metalType),
          sheenweight(_sheenweight),
          sheenColor(_sheenColor), sheenroughness(_sheenroughness),
          kt(_kt), ktweight(_ktweight),
          krcoatweight(_krcoatweight), krcoat(_krcoat), krcoatior(_krcoatior), krcoatgloss(_krcoatgloss),
          thinFilmThickness(_thinFilmThickness), thinFilmIOR(_thinFilmIOR),
          thinWalled(_thinWalled), bumpMap(_bumpMap)
    {
    }

    void StandardSurfaceMaterial::ComputeScatteringFunctions(SurfaceInteraction *isect) const
    {
        if (bumpMap)
        {
            BumpEvaluate(bumpMap, isect);
        }

        float eta = 1.f;
        if (ksweight > 0 && ktweight > 0)
        {
            eta = ksIOR;
        }
        isect->bsdf = std::make_shared<BSDF>(*isect, eta);

        // Prepare Parameters
        PrincipalSpectrum kdSpec = kd->Evaluate(*isect).SpectrumClamp();
        PrincipalSpectrum ksSpec = ks->Evaluate(*isect).SpectrumClamp();
        float kdevalrough = kdroughness->Evaluate(*isect);

        if ((kdweight > 0 && ksweight <= 0) ||
            (kdweight > 0 && ksweight > 0 && fresnelType == (int)FresnelType::WavelengthDependentMetallic))
        {
            // Remap [0-1] roughness to [0-90] sigma
            float sigma = kdevalrough * 90.f;
            std::unique_ptr<BxDF> diffuseBxdf = nullptr;
            if (!thinWalled)
            {
                diffuseBxdf = std::make_unique<OrenNayarBRDF>(kdSpec, kdweight, sigma);
            }
            else
            {
                diffuseBxdf = std::make_unique<DiffuseTransmission>(kdSpec, kdweight, sigma);
            }
            isect->bsdf->Add(diffuseBxdf);
        }

        float ksevalrough = ksroughness->Evaluate(*isect);
        float ksAnisotropic = ksanisotropic->Evaluate(*isect);

        float aspect = std::sqrt(1.f - ksAnisotropic * 0.9f);
        float ax = std::max(.001f, (ksevalrough * ksevalrough) / aspect);
        float ay = std::max(.001f, (ksevalrough * ksevalrough) * aspect);

        float ro = ((1.f - ksIOR) / (1.f + ksIOR)) * ((1.f - ksIOR) / (1.f + ksIOR));

        std::shared_ptr<Fresnel> specularFresnel = std::make_shared<SchlickDielectric>(ro);
        std::shared_ptr<MicrofacetDistribution> mainGGXdistribution = std::make_shared<GGXDistribution>(ax, ay);

        if (ksweight > 0)
        {
            if (ktweight > 0)
            {
                PrincipalSpectrum ktSpec = kt->Evaluate(*isect).SpectrumClamp();
                std::unique_ptr<BxDF> transBrdf = std::make_unique<FresnelSpecularBXDF>(ksSpec, ktSpec, 1.f, ksIOR);
                isect->bsdf->Add(transBrdf);
            }
            else
            {
                switch (fresnelType)
                {
                case (int)FresnelType::WavelengthDependentMetallic:
                {
                    std::pair<PrincipalSpectrum, PrincipalSpectrum> metalPair = GetMetalCoefficientsFromForge(metalType);
                    std::shared_ptr<Fresnel> fresnel = std::make_shared<FresnelConductor>(PrincipalSpectrum(1.f), metalPair.first, metalPair.second);
                    std::unique_ptr<BxDF> metallicBRDF = std::make_unique<MicrofacetReflectionBRDF>(mainGGXdistribution, fresnel, PrincipalSpectrum(1.f), ksweight);
                    isect->bsdf->Add(metallicBRDF);
                }
                break;
                case (int)FresnelType::Dielectric:
                case (int)FresnelType::Metallic:
                {
                    if (fresnelType == (int)FresnelType::Metallic)
                        specularFresnel = std::make_shared<SchlickMetallic>(ksSpec);

                    std::unique_ptr<BxDF> specularBrdf = std::make_unique<FresnelBlendedDiffuseSpecularBRDF>(kdSpec, kdweight, kdevalrough,
                                                                                                             ksSpec, ksweight, ro, specularFresnel, mainGGXdistribution);
                    isect->bsdf->Add(specularBrdf);
                }
                break;
                }
            }
        }

        if (thinFilmThickness > 0)
        {
            std::shared_ptr<Fresnel> thinFilmFresnel = std::make_shared<ThinFilmInteference>(thinFilmIOR, thinFilmThickness, ksIOR);
            std::unique_ptr<BxDF> thinFilmBRDF = std::make_unique<MicrofacetReflectionBRDF>(mainGGXdistribution, thinFilmFresnel, PrincipalSpectrum(1.f), 1.f);
            isect->bsdf->Add(thinFilmBRDF);
        }

        if (sheenweight > 0)
        {
            PrincipalSpectrum sheenCol = sheenColor->Evaluate(*isect).SpectrumClamp();
            float sheenevalrough = std::max(.001f, sheenroughness->Evaluate(*isect));
            std::shared_ptr<Fresnel> sheenFresnel = std::make_shared<FresnelNull>();
            std::shared_ptr<MicrofacetDistribution> sheenDistribution = std::make_shared<EstevezSheenDistribution>(sheenevalrough);
            std::unique_ptr<BxDF> sheenBRDF = std::make_unique<MicrofacetReflectionBRDF>(sheenDistribution, sheenFresnel, sheenCol, sheenweight);
            isect->bsdf->Add(sheenBRDF);
        }

        if (krcoatweight > 0)
        {
            PrincipalSpectrum krCoatSpec = krcoat->Evaluate(*isect).SpectrumClamp();
            std::unique_ptr<BxDF> clearcoatBRDF = std::make_unique<ClearcoatReflectionBRDF>(krCoatSpec, krcoatior, krcoatweight, krcoatgloss);
            isect->bsdf->Add(clearcoatBRDF);
        }
    }

} // namespace filianore
