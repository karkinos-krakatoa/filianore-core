#ifndef _INTEGRATOR_NORMAL_H
#define _INTEGRATOR_NORMAL_H

#include "../core/elemental.h"
#include "integrator.h"

namespace filianore {

class NormalIntegrator : public Integrator {
public:
    NormalIntegrator(const RenderParams &_renderParams)
        : Integrator(_renderParams) {
    }

    void prepare_the_renderer(const Scene &scene, Sampler &sampler);

    PrincipalSpectrum Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const;
};

} // namespace filianore

#endif