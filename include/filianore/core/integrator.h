#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include "material.h"
#include "primitive.h"
#include "renderparams.h"

namespace filianore {

class Integrator {
public:
    virtual ~Integrator() {}

    Integrator(const RenderParams &_renderParams)
        : renderParams(_renderParams) {}

    virtual void prepare_the_renderer(const Scene &scene, Sampler &sampler) = 0;
    virtual PrincipalSpectrum Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const = 0;

    RenderParams renderParams;
};

PrincipalSpectrum uniform_sample_all_lights(const Interaction &it, const Scene &scene, Sampler &sampler, bool handleMedia = false);

PrincipalSpectrum estimate_direct(const Interaction &it, const Vector2f &uShading, const Illuminant &illuminant, const Vector2f &uLight,
                                  const Scene &scene, Sampler &sampler, bool handleMedia = false, bool specular = false);

} // namespace filianore

#endif