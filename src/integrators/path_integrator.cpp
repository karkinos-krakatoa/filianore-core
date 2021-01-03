#include "filianore/integrators/path_integrator.h"
#include "filianore/core/camera.h"
#include "filianore/core/interaction.h"
#include "filianore/core/scene.h"
#include "filianore/core/bsdf.h"

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
        Color throughput(1.f);

        Ray ray(_ray);

        bool specularBounce = false;
        int bounces;
        float etaScale = 1.f;

        for (bounces = 0;; bounces++)
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

            isect.ComputeScatteringFunctions(ray);
            if (!isect.bsdf)
            {
                ray = isect.KindleRay(ray.dir);
                bounces--;
                continue;
            }

            if (isect.bsdf->NumComponents(BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) > 0.f)
            {
                Color Ld = UniformSampleAllLights(isect, scene, sampler, false) * throughput;
                L += Ld * throughput;
            }
        }

        return L;
    }
} // namespace filianore