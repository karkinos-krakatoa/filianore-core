#include "filianore/illuminants/diffusearea.h"
#include "filianore/maths/scalar.h"
#include "filianore/core/shape.h"

namespace filianore
{

    DiffuseAreaIlluminant::DiffuseAreaIlluminant(const Transform &_lightToWorld, const RGBSpectrum &_color, float _intensity,
                                                 short _decayRate, const RGBSpectrum &_shadowColor, const std::shared_ptr<Shape> &_shape)
        : AreaIlluminant(_lightToWorld, _decayRate, _shadowColor), color(_color), intensity(_intensity), shape(_shape), area(shape->Area())
    {
    }

    RGBSpectrum DiffuseAreaIlluminant::SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf, VisibilityEvaluator *visEval) const
    {
        Interaction ist = this->shape->Sample(isect, u, pdf);

        if (*pdf == 0 || (ist.p - isect.p).LengthSquared() == 0)
        {
            *pdf = 0.f;
            return RGBSpectrum(0.f);
        }

        *wi = (ist.p - isect.p).Neg().Normalize();
        *visEval = VisibilityEvaluator(isect, ist);

        return L(ist, *wi) * intensity;
    }

    RGBSpectrum DiffuseAreaIlluminant::Power() const
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

    RGBSpectrum DiffuseAreaIlluminant::L(const Interaction &isect, const StaticArray<float, 3> &w) const
    {
        return Dot(isect.n, w) > 0.f ? color * intensity : RGBSpectrum(0.f);
    }

} // namespace filianore