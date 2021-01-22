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
            isect->n = GeometricNormal(n);
            isect->ns = GeometricNormal(n); //ShadingNormal(u, v, isect->n);

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

    StaticArray<float, 3> Triangle::GeometricNormal(StaticArray<float, 3> &initialNormal) const
    {
        // StaticArray<float, 3> initNormal = initialNormal;

        // StaticArray<float, 3> p0 = v2.vertex - v1.vertex;
        // StaticArray<float, 3> p1 = v3.vertex - v1.vertex;
        // StaticArray<float, 3> faceNormal = initNormal;

        // StaticArray<float, 3> vertexNormal = (v1.normal + v2.normal + v3.normal) / 3.f;
        // float dot = Dot(faceNormal, v1.normal);

        // return (dot < 0.f) ? faceNormal.Neg() : faceNormal;
        return initialNormal.Normalize();
    }

    StaticArray<float, 3> Triangle::ShadingNormal(float u, float v, StaticArray<float, 3> geometricNormal) const
    {
        StaticArray<float, 3> ns = v1.normal * (1.f - u - v) + v2.normal * u + v3.normal * v;
        return ns;
    }

} // namespace filianore