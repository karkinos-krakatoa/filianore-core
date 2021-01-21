#include "filianore/core/scene.h"
#include "filianore/core/illuminant.h"
#include "filianore/core/primitive.h"

namespace filianore
{
    Scene::Scene(const std::shared_ptr<Primitive> &_scenePrims, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
        : scenePrims(_scenePrims), illuminants(_illuminants)
    {
        worldBound = scenePrims->WorldBound();

        for (const auto &illuminant : _illuminants)
        {
            illuminant->PrepareIlluminant(*this);
        }
    }

    bool Scene::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        return scenePrims->Intersect(ray, isect);
    }

    bool Scene::IntersectP(const Ray &ray) const
    {
        return scenePrims->IntersectP(ray);
    }
}