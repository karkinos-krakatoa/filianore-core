#ifndef _ILLUMINANT_DIRECTIONAL_H
#define _ILLUMINANT_DIRECTIONAL_H

#include "../core/illuminant.h"

namespace filianore
{

    class DirectionalIlluminant : public Illuminant
    {
    public:
        DirectionalIlluminant(const Transform &_lightToWorld, const StaticArray<float, 3> &_dirIllum,
                              const RGBSpectrum &_color, float _intensity, const RGBSpectrum &_shadowColor);

        RGBSpectrum SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                             VisibilityEvaluator *visEval) const;

        RGBSpectrum Power() const;

        void PrepareIlluminant(const Scene &scene);

        float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const;

    private:
        StaticArray<float, 3> dirIllum;
        const float intensity;
        const RGBSpectrum color;
        StaticArray<float, 3> worldCenter;
        float worldRadius;
    };

} // namespace filianore

#endif