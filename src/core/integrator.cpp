#include "filianore/core/integrator.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/interaction.h"
#include "filianore/core/sampler.h"
#include "filianore/core/sampling.h"
#include "filianore/core/scene.h"

namespace filianore {

PrincipalSpectrum uniform_sample_all_lights(const Interaction &it, const Scene &scene, Sampler &sampler, bool handleMedia) {
    PrincipalSpectrum L(0.f);

    for (size_t i = 0; i < scene.illuminants.size(); i++) {
        const std::shared_ptr<Illuminant> &illuminant = scene.illuminants[i];

        Vector2f uLight = sampler.get_2d();
        Vector2f uScattering = sampler.get_2d();

        L += estimate_direct(it, uScattering, *illuminant, uLight, scene, sampler, handleMedia);
    }

    return L;
}

PrincipalSpectrum estimate_direct(const Interaction &it, const Vector2f &uShading, const Illuminant &illuminant, const Vector2f &uLight,
                                  const Scene &scene, Sampler &sampler, bool handleMedia, bool specular) {
    BxDFType bsdfFlags = specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
    PrincipalSpectrum Ld(0);

    // MIS - Illuminant
    Vector3f wi;
    float illumPdf = 0, scatteringPdf = 0;
    VisibilityEvaluator visEval;

    PrincipalSpectrum Li = illuminant.sample_li(it, uLight, &wi, &illumPdf, &visEval);
    if (illumPdf > 0 && !Li.is_black()) {
        // evaluate BSDF for Illum sample
        PrincipalSpectrum f(0);

        if (it.is_surface_interaction()) {
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->evaluate(isect.wo, wi, bsdfFlags) * abs_dot(wi, isect.Shading.n);
            scatteringPdf = isect.bsdf->pdf(isect.wo, wi, bsdfFlags);
        }

        if (!f.is_black()) {
            if (!handleMedia) {
                if (!visEval.unoccluded(scene)) {
                    Li = illuminant.shadowColor;
                }
            }

            if (!Li.is_black()) {
                if (is_delta_illuminant(illuminant.types)) {
                    Ld += f * Li / illumPdf;
                } else {
                    float weight = power_heuristic(1, illumPdf, 1, scatteringPdf);
                    Ld += f * Li * weight / illumPdf;
                }
            }
        }
    }

    // MIS - BSDF
    if (!is_delta_illuminant(illuminant.types)) {
        PrincipalSpectrum f;
        bool sampledSpecular = false;
        if (it.is_surface_interaction()) {
            BxDFType sampledType;
            const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
            f = isect.bsdf->sample(isect.wo, &wi, uShading, &scatteringPdf, bsdfFlags, &sampledType);
            f *= abs_dot(wi, isect.Shading.n);
            sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
        } else {
            // Medium
        }

        if (!f.is_black() && scatteringPdf > 0) {
            float weight = 1.f;
            if (!sampledSpecular) {
                illumPdf = illuminant.pdf_li(it, wi);
                if (illumPdf == 0) {
                    return Ld;
                }
                weight = power_heuristic(1, scatteringPdf, 1, illumPdf);
            }

            SurfaceInteraction illuminantIsect;
            Ray ray = it.kindle_ray(wi);
            PrincipalSpectrum Tr(1.f);

            bool foundSurfaceInteraction = scene.intersect(ray, &illuminantIsect);

            PrincipalSpectrum Li;
            if (foundSurfaceInteraction) {
                if (illuminantIsect.primitive->get_area_illuminant() == &illuminant) {
                    Li = illuminantIsect.le(-wi);
                }
            } else {
                Li = illuminant.le(ray);
            }

            if (!Li.is_black()) {
                Ld += f * Li * Tr * weight / scatteringPdf;
            }
        }
    }

    return Ld;
}

} // namespace filianore