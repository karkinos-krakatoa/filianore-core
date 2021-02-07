#include "filianore/illuminants/point.h"
#include "filianore/maths/scalar.h"

namespace filianore
{

    PointIlluminant::PointIlluminant(const Transform &_lightToWorld, const RGBSpectrum &_color, float _intensity, short _decayRate, RGBSpectrum _shadowColor)
        : Illuminant(_lightToWorld, (int)IlluminantType::DeltaPoint, 1, _decayRate, _shadowColor), color(_color), intensity(_intensity)
    {
        posIllum = StaticArray<float, 3>(0.f);
        posIllum = _lightToWorld.PointTransform(posIllum);
    }

    RGBSpectrum PointIlluminant::SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                                          VisibilityEvaluator *visEval) const
    {
        *wi = (posIllum - isect.p).Normalize();
        *pdf = 1.f;

        *visEval = VisibilityEvaluator(isect, Interaction(posIllum, isect.time));

        return (color * intensity) / EvaluateDecayRate(posIllum - isect.p);
    }

    RGBSpectrum PointIlluminant::Power() const
    {
        return RGBSpectrum(intensity * 4.f * Pi<float>);
    }

    void PointIlluminant::PrepareIlluminant(const Scene &scene)
    {
    }

    float PointIlluminant::PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

} // namespace filianore