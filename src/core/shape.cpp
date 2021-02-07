#include "filianore/core/shape.h"
#include "filianore/core/interaction.h"

namespace filianore
{
    bool Shape::IntersectP(const Ray &ray) const
    {
        return Intersect(ray, 0);
    }

    float Shape::Pdf(const Interaction &isect) const
    {
        return 1.f / Area();
    }

    float Shape::Pdf(const Interaction &isect, const StaticArray<float, 3> &wi) const
    {
        Ray ray = isect.KindleRay(wi);
        SurfaceInteraction isc;
        if (!Intersect(ray, &isc))
        {
            return 0.f;
        }

        StaticArray<float, 3> wiNeg = wi;
        float pdf = (isect.p - isc.p).LengthSquared() / (AbsDot(isc.n, wiNeg.Neg()) * Area());
        if (std::isinf(pdf))
        {
            pdf = 0.f;
        }
        return pdf;
    }

    Interaction Shape::Sample(const Interaction &isect, const StaticArray<float, 2> &u, float *pdf) const
    {
        Interaction isc = Sample(u, pdf);
        StaticArray<float, 3> wi = isc.p - isect.p;
        if (wi.LengthSquared() == 0)
        {
            *pdf = 0.f;
        }
        else
        {
            wi = wi.Normalize();
            *pdf *= (isect.p - isc.p).LengthSquared() / AbsDot(isc.n, wi.Neg());
            if (std::isinf(*pdf))
            {
                *pdf = 0.f;
            }
        }
        return isc;
    }

} // namespace filianore