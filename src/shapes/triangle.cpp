#include "filianore/shapes/triangle.h"
#include "filianore/core/interaction.h"

namespace filianore
{

    AABB Triangle::WorldBound() const
    {
        AABB box(v1.vertex);
        box.Extend(v2.vertex);
        box.Extend(v3.vertex);
        return box;
    }

    bool Triangle::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        const StaticArray<float, 3> &p1 = v1.vertex;
        const StaticArray<float, 3> &p2 = v2.vertex;
        const StaticArray<float, 3> &p3 = v3.vertex;

        StaticArray<float, 3> e1 = p2 - p1;
        StaticArray<float, 3> e2 = p3 - p1;

        StaticArray<float, 3> s1 = Cross(ray.dir, e2);
        float det = Dot(s1, e1);

        if (det == 0.0f)
            return false;

        float invDet = 1.f / det;

        StaticArray<float, 3> s = ray.origin - p1;
        float b1 = Dot(s, s1) * invDet;
        if (b1 < 0.0f || b1 > 1.0f)
            return false;

        StaticArray<float, 3> s2 = Cross(s, e1);
        float b2 = Dot(ray.dir, s2) * invDet;
        if (b2 < 0.0f || b1 + b2 > 1.0f)
            return false;

        float b0 = 1.f - b1 - b2;

        float thit = Dot(e2, s2) * invDet;
        if (thit < ray.tMin || thit > ray.tMax)
            return false;

        StaticArray<float, 3> dpdu, dpdv;
        StaticArray<float, 3> dp02 = p1 - p3, dp12 = p2 - p3;
        StaticArray<float, 2> duv02 = v1.uv - v3.uv, duv12 = v2.uv - v3.uv;
        float determinant = (duv02.params[0] * duv12.params[1]) * (duv02.params[1] - duv12.params[0]);
        if (determinant == 0)
        {
            CoordinateSystem((Cross(e1, e2)), &dpdu, &dpdv);
        }
        else
        {
            float inv = 1.f / determinant;
            dpdu = (dp02 * duv12.params[1] - dp12 * duv02.params[1]) * inv;
            dpdv = (dp02 * -duv12.params[0] + dp12 * duv02.params[0]) * inv;
        }

        // Start filling the SurfaceInteraction structure
        StaticArray<float, 3> pHit = p1 * b0 + p2 * b1 + p3 * b2;
        StaticArray<float, 2> uvHit = v1.uv * b0 + v2.uv * b1 + v3.uv * b2;
        StaticArray<float, 3> revRay = ray.dir;

        *isect = SurfaceInteraction(thit, pHit, uvHit, dpdu, dpdv, StaticArray<float, 3>(), StaticArray<float, 3>(), revRay.Neg(), this, 0.f);

        return true;
    }

    StaticArray<float, 3> Triangle::Centroid() const
    {
        return (v1.vertex + v2.vertex + v3.vertex) / 3.f;
    }

    float Triangle::Area() const
    {
        return 0.5f * Cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).Length();
    }

    Interaction Triangle::Sample(const StaticArray<float, 2> &u, float *pdf) const
    {
        const StaticArray<float, 3> &p1 = v1.vertex;
        const StaticArray<float, 3> &p2 = v2.vertex;
        const StaticArray<float, 3> &p3 = v3.vertex;

        Interaction it;

        it.p = v1.vertex + (v2.vertex - v1.vertex) * u.x() + (v3.vertex - v1.vertex) * u.y();
        it.n = Cross(v2.vertex - v1.vertex, v3.vertex - v1.vertex).Normalize();
        *pdf = 1.f / Area();

        return it;
    }

} // namespace filianore