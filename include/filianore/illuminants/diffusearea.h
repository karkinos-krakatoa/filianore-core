#ifndef _ILLUMINANT_DIFFUSE_AREA_H
#define _ILLUMINANT_DIFFUSE_AREA_H

#include "../core/illuminant.h"

namespace filianore
{

    class DiffuseAreaIlluminant : public AreaIlluminant
    {
    public:
        DiffuseAreaIlluminant(const Transform &_lightToWorld, const RGBSpectrum &_color, float _intensity,
                              short _decayRate, const RGBSpectrum &_shadowColor, const std::shared_ptr<Shape> &_shape);

        RGBSpectrum SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                             VisibilityEvaluator *visEval) const;

        RGBSpectrum Power() const;

        void PrepareIlluminant(const Scene &scene);

        float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const;

        RGBSpectrum L(const Interaction &isect, const StaticArray<float, 3> &w) const;

    private:
        const float intensity;
        const RGBSpectrum color;
        const std::shared_ptr<Shape> shape;
        const float area;
    };

} // namespace filianore

#endif