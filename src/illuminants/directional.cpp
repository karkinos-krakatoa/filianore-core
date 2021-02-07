#include "filianore/illuminants/directional.h"
#include "filianore/maths/scalar.h"

namespace filianore
{

    DirectionalIlluminant::DirectionalIlluminant(const Transform &_lightToWorld, const StaticArray<float, 3> &_dirIllum,
                                                 const RGBSpectrum &_color, float _intensity, RGBSpectrum _shadowColor)
        : Illuminant(_lightToWorld, (int)IlluminantType::DeltaPoint, 1, 0.f, _shadowColor), color(_color), intensity(_intensity)
    {
        worldCenter = StaticArray<float, 3>(0.f);
        worldRadius = 1000.f;

        dirIllum = _lightToWorld.VectorTransform(_dirIllum).Neg().Normalize();
    }

    RGBSpectrum DirectionalIlluminant::SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                                                VisibilityEvaluator *visEval) const
    {
        *wi = dirIllum;
        *pdf = 1.f;

        StaticArray<float, 3> pOutside = isect.p + dirIllum * (2.f * worldRadius);
        *visEval = VisibilityEvaluator(isect, Interaction(pOutside, isect.time));

        return (color * intensity);
    }

    RGBSpectrum DirectionalIlluminant::Power() const
    {
        return RGBSpectrum(Pi<float> * worldRadius * worldRadius);
    }

    void DirectionalIlluminant::PrepareIlluminant(const Scene &scene)
    {
    }

    float DirectionalIlluminant::PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

} // namespace filianore