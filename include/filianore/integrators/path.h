#ifndef _INTEGRATOR_PATH_H
#define _INTEGRATOR_PATH_H

#include "../core/elemental.h"
#include "../core/integrator.h"

namespace filianore
{

    class PathIntegrator : public Integrator
    {
    public:
        PathIntegrator(int _maxDepth, std::shared_ptr<Sampler> _sampler, float _rrThreshold = 1);
        void PrepareTheRenderer(const Scene &scene, Sampler &sampler);
        Color Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const;

    private:
        const int maxDepth;
        const float rrThreshold;
        std::shared_ptr<Sampler> sampler;
    };

} // namespace filianore

#endif