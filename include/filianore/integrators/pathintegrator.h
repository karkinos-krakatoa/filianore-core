#ifndef _INTEGRATOR_PATH_H
#define _INTEGRATOR_PATH_H

#include "../core/elemental.h"
#include "../core/integrator.h"

namespace filianore
{

    class PathIntegrator : public Integrator
    {
    public:
        PathIntegrator(int _diffuseRayDepth, int _specularRayDepth, float _rrThreshold = 1.f)
            : diffuseRayDepth(_diffuseRayDepth), specularRayDepth(_specularRayDepth), rrThreshold(_rrThreshold)
        {
        }

        void PrepareTheRenderer(const Scene &scene, Sampler &sampler);

        PrincipalSpectrum Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const;

    private:
        const int diffuseRayDepth;
        const int specularRayDepth;
        const float rrThreshold;
    };

} // namespace filianore

#endif