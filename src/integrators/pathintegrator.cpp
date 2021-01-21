#include "filianore/integrators/pathintegrator.h"
#include "filianore/core/shadingcore.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/sampler.h"
#include "filianore/core/scene.h"

namespace filianore
{

    void PathIntegrator::PrepareTheRenderer(const Scene &scene, Sampler &sampler)
    {
    }

    RGBSpectrum PathIntegrator::Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const
    {
        RGBSpectrum L(0.f), throughput(1.f);
        Ray ray(_ray);
        bool specularBounce = false;
        int bounces;
        float etaScale = 1.f;

        for (bounces = 0;; ++bounces)
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

            if (isect.bsdf->NumComponents(BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) > 0)
            {
                RGBSpectrum Ld = throughput * UniformSampleAllLights(isect, scene, sampler, false);
                L += Ld;
            }

            // New Path
            StaticArray<float, 3> wo = ray.dir.Neg(), wi;
            float pdf;
            BxDFType flags;

            RGBSpectrum f = isect.bsdf->Sample(wo, &wi, sampler.Get2D(), &pdf, BSDF_ALL, &flags);

            if (f.IsBlack() || pdf == 0)
            {
                break;
            }

            throughput *= f * AbsDot(wi, isect.n) / pdf;
            specularBounce = (flags & BSDF_SPECULAR) != 0;

            if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION))
            {
                float eta = isect.bsdf->eta;
                etaScale *= (Dot(wo, isect.n) > 0) ? (eta * eta) : 1.f / (eta * eta);
            }

            ray = isect.KindleRay(wi);
        }

        return RGBSpectrum(1.f, 0.f, 0.f);
    }

} // namespace filianore