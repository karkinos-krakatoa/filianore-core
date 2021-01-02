#include "filianore/core/scene.h"
#include "filianore/core/illuminant.h"
#include "filianore/core/primitive.h"

namespace filianore
{
    Scene::Scene(std::shared_ptr<Primitive> _primitive, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
        : primitive(_primitive), illuminants(_illuminants)
    {
        sceneBound = _primitive->WorldBound();

        for (const auto &illuminant : _illuminants)
        {
            illuminant->PrepareIlluminant(*this);
        }
    }

    bool Scene::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        return primitive->Intersect(ray, isect);
    }

    bool Scene::IntersectP(const Ray &ray) const
    {
        return primitive->IntersectP(ray);
    }
}