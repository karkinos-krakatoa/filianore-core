#include "filianore/core/integrator.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/scene.h"
#include "filianore/core/interaction.h"
#include "filianore/core/sampler.h"
#include "filianore/core/sampling.h"

namespace filianore
{

    RGBSpectrum UniformSampleAllLights(const Interaction &it, const Scene &scene, Sampler &sampler, bool handleMedia)
    {
        RGBSpectrum L(0.f);

        for (size_t i = 0; i < scene.illuminants.size(); i++)
        {
            const std::shared_ptr<Illuminant> &illuminant = scene.illuminants[i];

            StaticArray<float, 2> uLight = sampler.Get2D();
            StaticArray<float, 2> uScattering = sampler.Get2D();

            L += EstimateDirect(it, uScattering, *illuminant, uLight, scene, sampler, handleMedia);
        }

        return L;
    }

    RGBSpectrum EstimateDirect(const Interaction &it, const StaticArray<float, 2> &uShading, const Illuminant &illuminant, const StaticArray<float, 2> &uLight,
                               const Scene &scene, Sampler &sampler, bool handleMedia, bool specular)
    {
        BxDFType bsdfFlags = specular ? BSDF_ALL : BxDFType(BSDF_ALL & ~BSDF_SPECULAR);
        RGBSpectrum Ld(0);

        // MIS - Illuminant
        StaticArray<float, 3> wi;
        float illumPdf = 0, scatteringPdf = 0;
        VisibilityEvaluator visEval;

        RGBSpectrum Li = illuminant.SampleLi(it, uLight, &wi, &illumPdf, &visEval);
        if (illumPdf > 0 && !Li.IsBlack())
        {
            // Evaluate BSDF for Illum Sample
            RGBSpectrum f(0);

            if (it.IsSurfaceInteraction())
            {
                const SurfaceInteraction &isect = (const SurfaceInteraction &)it;
                f = isect.bsdf->Evaluate(isect.wo, wi, bsdfFlags) * AbsDot(wi, isect.n);
                scatteringPdf = isect.bsdf->Pdf(isect.wo, wi, bsdfFlags);
            }

            if (!f.IsBlack())
            {
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

        return Ld;
    }

} // namespace filianore