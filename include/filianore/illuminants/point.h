#ifndef _ILLUMINANT_POINT_H
#define _ILLUMINANT_POINT_H

#include "../core/illuminant.h"

namespace filianore
{

    class PointIlluminant : public Illuminant
    {
    public:
        PointIlluminant(const Transform &_lightToWorld, const RGBSpectrum &_color, float _intensity, short _decayRate, RGBSpectrum _shadowColor)
            : Illuminant(_lightToWorld, (int)IlluminantType::DeltaPoint, 1, _decayRate, _shadowColor), color(_color), intensity(_intensity)
        {
            posIllum = StaticArray<float, 3>(0.f);
            posIllum = _lightToWorld.PointTransform(posIllum);
        }

        RGBSpectrum SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                             VisibilityEvaluator *visEval) const;

        RGBSpectrum Power() const;

        void PrepareIlluminant(const Scene &scene);

        float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const;

    private:
        StaticArray<float, 3> posIllum;
        const float intensity;
        const RGBSpectrum color;
    };

} // namespace filianore

#endif