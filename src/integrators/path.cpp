#include "filianore/integrators/path.h"
#include "filianore/core/camera.h"
#include "filianore/core/interaction.h"
#include "filianore/core/scene.h"

namespace filianore
{
    PathIntegrator::PathIntegrator(int _maxDepth, std::shared_ptr<Sampler> _sampler, float _rrThreshold)
        : maxDepth(_maxDepth), sampler(_sampler), rrThreshold(_rrThreshold)
    {
    }

    void PathIntegrator::PrepareTheRenderer(const Scene &scene, Sampler &sampler)
    {
    }

    Color PathIntegrator::Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const
    {
        Color L(0.f);
        Color throughput(0.f);

        Ray ray(_ray);

        bool specularBounce = false;
        int bounces;
        float etaScale = 1;

        for (int bounces = 0;; bounces++)
        {
            SurfaceInteraction isect;
            bool hitFound = scene.Intersect(ray, &isect);

            if (bounces == 0 || specularBounce)
            {
                if (hitFound)
                {
                    L += throughput * isect.Le(ray.dir.Neg());
                }
            }

            if (!hitFound || bounces >= maxDepth)
            {
                break;
            }
        }

        return L;
    }
}