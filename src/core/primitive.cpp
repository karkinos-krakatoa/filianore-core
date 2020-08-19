#include "filianore/core/primitive.h"
#include "filianore/core/interaction.h"
#include "filianore/core/aabb.h"

namespace filianore
{

    AABB GeometricPrimitive::WorldBound() const
    {
        return shape->WorldBound();
    }

    bool GeometricPrimitive::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        if (!shape->Intersect(ray, isect))
        {
            return false;
        }
        ray.tMax = isect->t;
        isect->primitive = this;

        return true;
    }

    bool GeometricPrimitive::IntersectP(const Ray &ray) const
    {
        return shape->IntersectP(ray);
    }

    StaticArray<float, 3> GeometricPrimitive::Centroid() const
    {
        return shape->Centroid();
    }

} // namespace filianore