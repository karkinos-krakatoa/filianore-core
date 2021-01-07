#include "filianore/illuminants/point.h"
#include "filianore/maths/scalar.h"

namespace filianore
{

    Color PointIlluminant::SampleLi(const Interaction &ref, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf, VisibilityEvaluator *vis) const
    {
        *wi = (illumPosition - ref.p).Normalize();
        *pdf = 1.f;
        *vis = VisibilityEvaluator(ref, Interaction(illumPosition, ref.time));
        return (I * Iw) / ((illumPosition - ref.p).LengthSquared());
    }

    Color PointIlluminant::Power() const
    {
        return I * Iw * 4.f * Pi<float>;
    }

    Color PointIlluminant::Le(const Ray &ray) const
    {
        return Color(0.f);
    }

    float PointIlluminant::PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

} // namespace filianore