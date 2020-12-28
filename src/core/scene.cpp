#include "filianore/core/scene.h"

namespace filianore
{
    bool Scene::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        return primitive->Intersect(ray, isect);
    }

    bool Scene::IntersectP(const Ray &ray) const
    {
        return primitive->IntersectP(ray);
    }
}