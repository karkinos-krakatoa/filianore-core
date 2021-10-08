#include "filianore/core/integrator.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/scene.h"
#include "filianore/core/interaction.h"
#include "filianore/core/sampler.h"
#include "filianore/core/sampling.h"

namespace filianore
{

    PrincipalSpectrum UniformSampleAllLights(const Interaction &it, const Scene &scene, Sampler &sampler, bool handleMedia)
    {
        PrincipalSpectrum L(0.f);

        for (size_t i = 0; i < scene.illuminants.size(); i++)
        {
            const std::shared_ptr<Illuminant> &illuminant = scene.illuminants[i];

            StaticArray<float, 2> uLight = sampler.Get2D();
            StaticArray<float, 2> uScattering = sampler.Get2D();

            L += EstimateDirect(it, uScattering, *illuminant, uLight, scene, sampler, handleMedia);
        }

        return L;
    }

    PrincipalSpectrum EstimateDirect(const Interaction &it, const StaticArray<float, 2> &uShading, const Illuminant &illuminant, const StaticArray<float, 2> &uLight,
                                     const Scene &scene, Sampler &sampler, bool handleMedia, bool specular)
    {
        BxDFType bsdfFlags = specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
        PrincipalSpectrum Ld(0);

        // MIS - Illuminant
        StaticArray<float, 3> wi;
        float illumPdf = 0, scatteringPdf = 0;
        VisibilityEvaluator visEval;

        PrincipalSpectrum Li = illuminant.SampleLi(it, uLight, &wi, &illumPdf, &visEval);
        if (illumPdf > 0 && !Li.IsBlack())
        {
            // Evaluate BSDF for Illum Sample
            PrincipalSpectrum f(0);

            if (it.IsSurfaceInteraction())
            {
                const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
                f = isect.bsdf->Evaluate(isect.wo, wi, bsdfFlags) * AbsDot(wi, isect.Shading.n);
                scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
            }

            if (!f.IsBlack())
            {
                if (!handleMedia)
                {
                    if (!visEval.Unoccluded(scene))
                    {
                        Li = illuminant.shadowColor;
                    }
                }

                if (!Li.IsBlack())
                {
                    if (IsDeltaIlluminant(illuminant.types))
                    {
                        Ld += f * Li / illumPdf;
                    }
                    else
                    {
                        float weight = PowerHeuristic(1, illumPdf, 1, scatteringPdf);
                        Ld += f * Li * weight / illumPdf;
                    }
                }
            }
        }

        // MIS - BSDF
        if (!IsDeltaIlluminant(illuminant.types))
        {
            PrincipalSpectrum f;
            bool sampledSpecular = false;
            if (it.IsSurfaceInteraction())
            {
                BxDFType sampledType;
                const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
                f = isect.bsdf->Sample(isect.wo, &wi, uShading, &scatteringPdf, bsdfFlags, &sampledType);
                f *= AbsDot(wi, isect.Shading.n);
                sampledSpecular = (sampledType & BSDF_SPECULAR) != 0;
            }
            else
            {
                // Medium
            }

            if (!f.IsBlack() && scatteringPdf > 0)
            {
                float weight = 1.f;
                if (!sampledSpecular)
                {
                    illumPdf = illuminant.PdfLi(it, wi);
                    if (illumPdf == 0)
                    {
                        return Ld;
                    }
                    weight = PowerHeuristic(1, scatteringPdf, 1, illumPdf);
                }

                SurfaceInteraction illuminantIsect;
                Ray ray = it.KindleRay(wi);
                PrincipalSpectrum Tr(1.f);

                bool foundSurfaceInteraction = scene.Intersect(ray, &illuminantIsect);

                PrincipalSpectrum Li;
                if (foundSurfaceInteraction)
                {
                    if (illuminantIsect.primitive->GetAreaIlluminant() == &illuminant)
                    {
                        Li = illuminantIsect.Le(wi.Neg());
                    }
                }
                else
                {
                    Li = illuminant.Le(ray);
                }

                if (!Li.IsBlack())
                {
                    Ld += f * Li * Tr * weight / scatteringPdf;
                }
            }
        }

        return Ld;
    }

} // namespace filianore