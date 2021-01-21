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

        Scene(const std::shared_ptr<Primitive> &_scenePrims, const std::vector<std::shared_ptr<Illuminant>> &_illuminants);

        const AABB &WorldBound() const
        {
            return worldBound;
        }

        bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;

        bool IntersectP(const Ray &ray) const;

        std::vector<std::shared_ptr<Illuminant>> illuminants;

    private:
        const std::shared_ptr<Primitive> scenePrims;
        AABB worldBound;
    };

} // namespace filianore

#endif