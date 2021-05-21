#ifndef _SCENE_H
#define _SCENE_H

#include "primitive.h"
#include "illuminant.h"
#include "aabb.h"
#include <vector>

namespace filianore
{

    class Scene
    {
    public:
        Scene() {}

        Scene(
            const std::shared_ptr<RayTraverser> &_traverser,
            const std::shared_ptr<PrimitiveIntersector> &_primIntersector,
            const std::vector<std::shared_ptr<Illuminant>> &_illuminants);

        bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;

        std::vector<std::shared_ptr<Illuminant>> illuminants;

    private:
        const std::shared_ptr<Primitive> scenePrims;
        std::shared_ptr<RayTraverser> traverser;
        std::shared_ptr<PrimitiveIntersector> primIntersector;
    };

} // namespace filianore

#endif