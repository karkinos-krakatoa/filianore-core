#include "filianore/illuminants/diffusearea.h"
#include "filianore/maths/scalar.h"
#include "filianore/core/shape.h"

namespace filianore
{

    DiffuseAreaIlluminant::DiffuseAreaIlluminant(const Transform &_lightToWorld, const PrincipalSpectrum &_color, float _intensity,
                                                 short _decayRate, const PrincipalSpectrum &_shadowColor, const std::shared_ptr<Shape> &_shape)
        : AreaIlluminant(_lightToWorld, _decayRate, _shadowColor), color(_color), intensity(_intensity), shape(_shape), area(shape->Area())
    {
    }

    PrincipalSpectrum DiffuseAreaIlluminant::SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf, VisibilityEvaluator *visEval) const
    {
        Interaction ist = shape->Sample(isect, u, pdf);

        if (*pdf == 0 || (ist.p - isect.p).LengthSquared() == 0)
        {
            *pdf = 0.f;
            return PrincipalSpectrum(0.f);
        }

        *wi = (ist.p - isect.p).Normalize();
        *visEval = VisibilityEvaluator(isect, ist);

        return L(ist, StaticArray<float, 3>(-wi->x(), -wi->y(), -wi->z())) * intensity / EvaluateDecayRate(ist.p - isect.p);
    }

    PrincipalSpectrum DiffuseAreaIlluminant::Power() const
    {
        return color * intensity * area * Pi<float>;
    }

    void DiffuseAreaIlluminant::PrepareIlluminant(const Scene &scene)
    {
    }

    float DiffuseAreaIlluminant::PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const
    {
        return shape->Pdf(ref, wi);
    }

    PrincipalSpectrum DiffuseAreaIlluminant::L(const Interaction &isect, const StaticArray<float, 3> &w) const
    {
        return Dot(isect.n, w) > 0.f ? color * intensity : PrincipalSpectrum(0.f);
    }

} // namespace filianore