#include "normalintegrator.h"
#include "../color/spectrumoperations.h"
#include "../core/scene.h"
#include "../sampling/sampler.h"

namespace filianore {

void NormalIntegrator::prepare_the_renderer(const Scene &scene, Sampler &sampler) {
}

PrincipalSpectrum NormalIntegrator::Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const {
    PrincipalSpectrum L(0.f), throughput(1.f);
    Ray ray(_ray);
    bool specularBounce = false;
    int bounces;
    float etaScale = 1.f;

    SurfaceInteraction isect;
    bool hitFound = scene.intersect(ray, &isect);

    if (!hitFound) {
        return PrincipalSpectrum(0.f);
    }

    return from_reflectance_rgb(isect.n);
}

} // namespace filianore