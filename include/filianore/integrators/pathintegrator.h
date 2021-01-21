#ifndef _INTEGRATOR_PATH_H
#define _INTEGRATOR_PATH_H

#include "../core/elemental.h"
#include "../core/integrator.h"

namespace filianore
{

    class PathIntegrator : public Integrator
    {
    public:
        PathIntegrator(int _maxDepth, float _rrThreshold = 1.f)
            : maxDepth(_maxDepth), rrThreshold(_rrThreshold)
        {
        }

        void PrepareTheRenderer(const Scene &scene, Sampler &sampler);

        Spectrum<float> Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const;

    private:
        const int maxDepth;
        const float rrThreshold;
    };

} // namespace filianore

#endif