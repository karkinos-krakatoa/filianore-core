#include "filianore/illuminants/point.h"
#include "filianore/maths/scalar.h"

namespace filianore
{

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