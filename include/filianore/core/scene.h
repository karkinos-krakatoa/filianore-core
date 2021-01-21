#ifndef _SCENE_H
#define _SCENE_H

#include "primitive.h"
#include "illuminant.h"
#include "aabb.h"

namespace filianore
{

    class Scene
    {
    public:
        Scene() {}

        Scene(const std::shared_ptr<Primitive> &_scenePrims)
            : scenePrims(_scenePrims), illuminants(NULL)
        {
            worldBound = _scenePrims->WorldBound();
        }

        Scene(const std::shared_ptr<Primitive> &_scenePrims, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
            : scenePrims(_scenePrims), illuminants(_illuminants)
        {
            worldBound = _scenePrims->WorldBound();
        }

        const AABB &WorldBound() const
        {
            return worldBound;
        }

        bool Intersect(const Ray &ray, SurfaceInteraction *isect) const
        {
            return scenePrims->Intersect(ray, isect);
        }

        bool IntersectP(const Ray &ray) const
        {
            return scenePrims->IntersectP(ray);
        }

        std::vector<std::shared_ptr<Illuminant>> illuminants;

    private:
        std::shared_ptr<Primitive> scenePrims;
        AABB worldBound;
    };

} // namespace filianore

#endif