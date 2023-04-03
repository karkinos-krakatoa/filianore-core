#include "pathintegrator.h"
#include "../core/scene.h"
#include "../core/shadingcore.h"
#include "../sampling/sampler.h"
#include "../shading/bsdf.h"

namespace filianore {

void PathIntegrator::prepare_the_renderer(const Scene &scene, Sampler &sampler) {
}

PrincipalSpectrum PathIntegrator::Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const {
    PrincipalSpectrum L(0.f), throughput(1.f);
    Ray ray(_ray);
    bool specularBounce = false;
    int bounces;
    float etaScale = 1.f;

    for (bounces = 0;; ++bounces) {
        SurfaceInteraction isect;
        bool hitFound = scene.intersect(ray, &isect);

        if (bounces == 0 || specularBounce) {
            if (hitFound) {
                L += throughput * isect.le(-ray.dir);
            }
        }

        if (!hitFound) {
            break;
        }

        isect.compute_scattering_functions(ray);
        if (!isect.bsdf) {
            ray = isect.kindle_ray(ray.dir);
            bounces--;
            continue;
        }

        if (isect.bsdf->num_components(BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) > 0) {
            PrincipalSpectrum Ld = throughput * uniform_sample_all_lights(isect, scene, sampler, false);
            L += Ld;
        }

        // New Path
        Vector3f wo = -ray.dir, wi;
        float pdf;
        BxDFType flags;

        PrincipalSpectrum f = isect.bsdf->sample(wo, &wi, sampler.get_2d(), &pdf, BSDF_ALL, &flags);
        if (f.is_black() || pdf == 0) {
            break;
        }

        if (((renderParams.diffuseRayDepth == bounces) && (flags & BSDF_DIFFUSE))) {
            break;
        }

        if (((renderParams.reflectionRayDepth == bounces) && (flags & BSDF_SPECULAR)) || (renderParams.reflectionRayDepth == bounces && (flags & BSDF_GLOSSY))) {
            break;
        }

        if (((renderParams.transmissionRayDepth == bounces) && (flags & BSDF_TRANSMISSION))) {
            break;
        }

        throughput *= f * abs_dot(wi, isect.Shading.n) / pdf;
        specularBounce = (flags & BSDF_SPECULAR) != 0;

        if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
            float eta = isect.bsdf->eta;
            etaScale *= (dot(wo, isect.n) > 0) ? (eta * eta) : 1.f / (eta * eta);
        }

        ray = isect.kindle_ray(wi);

        PrincipalSpectrum rrBeta = throughput * etaScale;
        if (rrBeta.max_component_value() < rrThreshold && bounces > 3) {
            float q = std::max(0.05f, 1.f - rrBeta.max_component_value());
            if (sampler.get_1d() < q)
                break;
            throughput /= 1.f - q;
        }
    }

    return L;
}

} // namespace filianore