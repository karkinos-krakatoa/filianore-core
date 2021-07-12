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
        StaticArray<float, 3> e1 = v2.vertex - v1.vertex;
        StaticArray<float, 3> e2 = v3.vertex - v1.vertex;

        StaticArray<float, 3> n = Cross(e1, e2);

        float det = -Dot(ray.dir, n);

        StaticArray<float, 3> ao = ray.origin - v1.vertex;
        StaticArray<float, 3> dao = Cross(ao, ray.dir);

        float u = Dot(e2, dao) / det;
        float v = -Dot(e1, dao) / det;
        float t = Dot(ao, n) / det;

        if ((t < ray.tMax && t > ray.tMin) && u > 0 && v > 0 && (u + v) < 1.f)
        {
            StaticArray<float, 3> revRay = ray.dir;
            *isect = SurfaceInteraction(t, StaticArray<float, 3>(), StaticArray<float, 3>(), StaticArray<float, 2>(), revRay.Neg(), this, 0);

            isect->p = ray.PointAtT(t);
            isect->uv = StaticArray<float, 2>(u, v);
            isect->n = Cross(e1, e2).Normalize();

            return true;
        }

        return false;
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
        float uu = std::sqrt(u.x());
        float u1 = 1.f - uu;
        float u2 = u.y() * uu;

        Interaction it;

        it.p = v1.vertex * u1 + v2.vertex * u2 + v3.vertex * (1.f - u1 - u2);
        it.n = Cross(v2.vertex - v1.vertex, v3.vertex - v1.vertex).Normalize();

        *pdf = 1.f / Area();

        return it;
    }

} // namespace filianore