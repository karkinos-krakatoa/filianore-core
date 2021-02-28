#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include "primitive.h"
#include "material.h"
#include "renderparams.h"

namespace filianore
{

    class Integrator
    {
    public:
        virtual ~Integrator() {}

        Integrator(const RenderParams &_renderParams)
            : renderParams(_renderParams) {}

        virtual void PrepareTheRenderer(const Scene &scene, Sampler &sampler) = 0;
        virtual PrincipalSpectrum Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const = 0;

        RenderParams renderParams;
    };

    PrincipalSpectrum UniformSampleAllLights(const Interaction &it, const Scene &scene, Sampler &sampler, bool handleMedia = false);

    PrincipalSpectrum EstimateDirect(const Interaction &it, const StaticArray<float, 2> &uShading, const Illuminant &illuminant, const StaticArray<float, 2> &uLight,
                                     const Scene &scene, Sampler &sampler, bool handleMedia = false, bool specular = false);

} // namespace filianore

#endif