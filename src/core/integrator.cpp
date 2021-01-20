#include "filianore/core/integrator.h"
#include "filianore/core/primitive.h"
#include "filianore/core/illuminant.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/sampler.h"
#include "filianore/core/sampling.h"
#include "filianore/core/material.h"
#include "filianore/core/scene.h"

namespace filianore
{

    Color UniformSampleAllLights(const Interaction &it, const Scene &scene, Sampler &sampler, bool handleMedia)
    {
        Color L(0.f);

        for (size_t i = 0; i < scene.illuminants.size(); i++)
        {
            const std::shared_ptr<Illuminant> &illuminant = scene.illuminants[i];

            StaticArray<float, 2> uLight = sampler.Get2D();
            StaticArray<float, 2> uScattering = sampler.Get2D();

            L += EstimateDirect(it, uScattering, *illuminant, uLight, scene, sampler, handleMedia);
        }

        return L;
    }

    Color EstimateDirect(const Interaction &it, const StaticArray<float, 2> &uShading, const Illuminant &illuminant, const StaticArray<float, 2> &uLight,
                         const Scene &scene, Sampler &sampler, bool handleMedia, bool specular)
    {
        BxDFType bsdfFlags = specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);

        Color Ld(0.f);

        // Illuminant Sampling
        StaticArray<float, 3> wi;
        float illuminantPdf = 0.f, scatteringPdf = 0.f;
        VisibilityEvaluator visEval;
        Color Li = illuminant.SampleLi(it, uLight, &wi, &illuminantPdf, &visEval);
        if (illuminantPdf > 0.f && !Li.IsZero())
        {
            Color f;
            if (it.IsSurfaceInteraction())
            {
                const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
                if (isect.bsdf)
                {
                    f = isect.bsdf->EvaluateBSDF(isect.wo, wi, bsdfFlags) * AbsDot(wi, isect.n);
                    scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
                }
            }

            if (!f.IsZero())
            {
                if (visEval.Occluded(scene))
                {
                    Li = StaticArray<float, 3>(0.f, 0.f, 0.f);
                }

                if (!Li.IsZero())
                {
                    if (IsDeltaIlluminant(illuminant.types))
                    {
                        Ld = (f * Li) / illuminantPdf;
                    }
                    else
                    {
                        float weight = PowerHeuristic(1.f, illuminantPdf, 1.f, scatteringPdf);
                        Ld = (f * Li * weight) / illuminantPdf;
                    }
                }
            }
        }

        // // BSDF Sampling
        // if (!IsDeltaIlluminant(illuminant.types))
        // {
        //     Color f;
        //     bool sampledSpecular = false;
        //     if (it.IsSurfaceInteraction())
        //     {
        //         BxDFType sampledType;
        //         const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
        //         f = isect.bsdf->SampleBSDF(isect.wo, &wi, uShading, &scatteringPdf, bsdfFlags, &sampledType);
        //         f *= AbsDot(wi, isect.n);
        //         sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
        //     }

        //     if (scatteringPdf > 0.f && !f.IsZero())
        //     {
        //         float weight = 1.f;
        //         if (!sampledSpecular)
        //         {
        //             illuminantPdf = illuminant.PdfLi(it, wi);
        //             if (illuminantPdf == 0.f)
        //             {
        //                 return Ld;
        //             }
        //             weight = PowerHeuristic(1.f, scatteringPdf, 1.f, illuminantPdf);
        //         }

        //         SurfaceInteraction illumSect;
        //         Ray ray = it.KindleRay(wi);
        //         Color Tr(1.f);
        //         bool foundSurfaceIsect = handleMedia ? false : scene.Intersect(ray, &illumSect);

        //         Color Li(0.f);
        //         if (foundSurfaceIsect)
        //         {
        //             Li = 0.f;
        //         }

        //         if (!Li.IsZero())
        //         {
        //             Ld = (f * Li * Tr * weight) / scatteringPdf;
        //         }
        //     }
        // }

        return Ld;
    }

} // namespace filianore