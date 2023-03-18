#ifndef _SCENE_H
#define _SCENE_H

#include "aabb.h"
#include "illuminant.h"
#include "primitive.h"
#include <vector>

namespace filianore {

class Scene {
public:
    Scene() {}

    Scene(const std::shared_ptr<Primitive> &_scenePrims, const std::vector<std::shared_ptr<Illuminant>> &_illuminants);

    const AABB &world_bound() const {
        return worldBound;
    }

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;

    bool intersect_p(const Ray &ray) const;

    std::vector<std::shared_ptr<Illuminant>> illuminants;

private:
    const std::shared_ptr<Primitive> scenePrims;
    AABB worldBound;
};

} // namespace filianore

#endif