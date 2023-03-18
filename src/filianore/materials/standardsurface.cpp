#include "standardsurface.h"

#include "../textures/imagemap.h"

#include "../shading/bxdfs/clearcoat.h"
#include "../shading/bxdfs/diffusetransmission.h"
#include "../shading/bxdfs/lambert.h"
#include "../shading/bxdfs/microfacetreflection.h"
#include "../shading/bxdfs/orennayar.h"

#include "../color/metallurgy.h"
#include "../color/spectrumoperations.h"

#include "../shading/fresnel/fresnelconductor.h"
#include "../shading/fresnel/fresnelnull.h"
#include "../shading/fresnel/schlickdielectric.h"
#include "../shading/fresnel/schlickmetallic.h"
#include "../shading/fresnel/thinfilm.h"

#include "../shading/microfacets/estevez.h"
#include "../shading/microfacets/ggx.h"

#include "../shading/closures/fresnelblendeddiffspec.h"
#include "../shading/closures/fresnelspecular.h"
#include "../shading/closures/microfacettransmission.h"

#include "../core/bsdf.h"
#include "../core/interaction.h"
#include "../core/texture.h"

namespace filianore {

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
      thinWalled(_thinWalled), bumpMap(_bumpMap) {
}

void StandardSurfaceMaterial::compute_scattering_functions(SurfaceInteraction *isect) const {
    if (bumpMap) {
        bump_evaluate(bumpMap, isect);
    }

    float eta = 1.f;
    if (ksweight > 0 && ktweight > 0) {
        eta = ksIOR;
    }
    isect->bsdf = std::make_shared<BSDF>(*isect, eta);

    // Prepare Parameters
    PrincipalSpectrum kdSpec = kd->evaluate(*isect).spectrum_clamp();
    PrincipalSpectrum ksSpec = ks->evaluate(*isect).spectrum_clamp();
    float kdevalrough = kdroughness->evaluate(*isect);

    if ((kdweight > 0 && ksweight <= 0) ||
        (kdweight > 0 && ksweight > 0 && fresnelType == (int)FresnelType::WavelengthDependentMetallic)) {
        // Remap [0-1] roughness to [0-90] sigma
        float sigma = kdevalrough * 90.f;
        std::unique_ptr<BxDF> diffuseBxdf = nullptr;
        if (!thinWalled) {
            diffuseBxdf = std::make_unique<OrenNayarBRDF>(kdSpec, kdweight, sigma);
        } else {
            diffuseBxdf = std::make_unique<DiffuseTransmission>(kdSpec, kdweight, sigma);
        }
        isect->bsdf->add(diffuseBxdf);
    }

    float ksevalrough = ksroughness->evaluate(*isect);
    float ksAnisotropic = ksanisotropic->evaluate(*isect);

    float aspect = std::sqrt(1.f - ksAnisotropic * 0.9f);
    float ax = std::max(.001f, (ksevalrough * ksevalrough) / aspect);
    float ay = std::max(.001f, (ksevalrough * ksevalrough) * aspect);

    float ro = ((1.f - ksIOR) / (1.f + ksIOR)) * ((1.f - ksIOR) / (1.f + ksIOR));

    std::shared_ptr<Fresnel> specularFresnel = std::make_shared<SchlickDielectric>(ro);
    std::shared_ptr<MicrofacetDistribution> mainGGXdistribution = std::make_shared<GGXDistribution>(ax, ay);

    if (ksweight > 0) {
        if (ktweight > 0) {
            PrincipalSpectrum ktSpec = kt->evaluate(*isect).spectrum_clamp();
            std::unique_ptr<BxDF> transBrdf = std::make_unique<FresnelSpecularBXDF>(ksSpec, ktSpec, 1.f, ksIOR);
            isect->bsdf->add(transBrdf);
        } else {
            switch (fresnelType) {
            case (int)FresnelType::WavelengthDependentMetallic: {
                std::pair<PrincipalSpectrum, PrincipalSpectrum> metalPair = GetMetalCoefficientsFromForge(metalType);
                std::shared_ptr<Fresnel> fresnel = std::make_shared<FresnelConductor>(PrincipalSpectrum(1.f), metalPair.first, metalPair.second);
                std::unique_ptr<BxDF> metallicBRDF = std::make_unique<MicrofacetReflectionBRDF>(mainGGXdistribution, fresnel, PrincipalSpectrum(1.f), ksweight);
                isect->bsdf->add(metallicBRDF);
            } break;
            case (int)FresnelType::Dielectric:
            case (int)FresnelType::Metallic: {
                if (fresnelType == (int)FresnelType::Metallic)
                    specularFresnel = std::make_shared<SchlickMetallic>(ksSpec);

                std::unique_ptr<BxDF> specularBrdf = std::make_unique<FresnelBlendedDiffuseSpecularBRDF>(kdSpec, kdweight, kdevalrough,
                                                                                                         ksSpec, ksweight, ro, specularFresnel, mainGGXdistribution);
                isect->bsdf->add(specularBrdf);
            } break;
            }
        }
    }

    if (thinFilmThickness > 0) {
        std::shared_ptr<Fresnel> thinFilmFresnel = std::make_shared<ThinFilmInteference>(thinFilmIOR, thinFilmThickness, ksIOR);
        std::unique_ptr<BxDF> thinFilmBRDF = std::make_unique<MicrofacetReflectionBRDF>(mainGGXdistribution, thinFilmFresnel, PrincipalSpectrum(1.f), 1.f);
        isect->bsdf->add(thinFilmBRDF);
    }

    if (sheenweight > 0) {
        PrincipalSpectrum sheenCol = sheenColor->evaluate(*isect).spectrum_clamp();
        float sheenevalrough = std::max(.001f, sheenroughness->evaluate(*isect));
        std::shared_ptr<Fresnel> sheenFresnel = std::make_shared<FresnelNull>();
        std::shared_ptr<MicrofacetDistribution> sheenDistribution = std::make_shared<EstevezSheenDistribution>(sheenevalrough);
        std::unique_ptr<BxDF> sheenBRDF = std::make_unique<MicrofacetReflectionBRDF>(sheenDistribution, sheenFresnel, sheenCol, sheenweight);
        isect->bsdf->add(sheenBRDF);
    }

    if (krcoatweight > 0) {
        PrincipalSpectrum krCoatSpec = krcoat->evaluate(*isect).spectrum_clamp();
        std::unique_ptr<BxDF> clearcoatBRDF = std::make_unique<ClearcoatReflectionBRDF>(krCoatSpec, krcoatior, krcoatweight, krcoatgloss);
        isect->bsdf->add(clearcoatBRDF);
    }
}

} // namespace filianore
