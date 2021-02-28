#ifndef _INTEGRATOR_PATH_H
#define _INTEGRATOR_PATH_H

#include "../core/elemental.h"
#include "../core/integrator.h"

namespace filianore
{

    class PathIntegrator : public Integrator
    {
    public:
        PathIntegrator(const RenderParams &_renderParams, float _rrThreshold = 1.f)
            : Integrator(_renderParams), rrThreshold(_rrThreshold)
        {
        }

        void PrepareTheRenderer(const Scene &scene, Sampler &sampler);

        PrincipalSpectrum Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const;

    private:
        const float rrThreshold;
    };

} // namespace filianore

#endif