#include "scene.h"
#include "illuminant.h"
#include "primitive.h"

namespace filianore {
Scene::Scene(const std::shared_ptr<Primitive> &_scenePrims, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
    : scenePrims(_scenePrims), illuminants(_illuminants) {
    worldBound = scenePrims->world_bound();

    for (const auto &illuminant : illuminants) {
        illuminant->prepare_illuminant(*this);
    }
}

bool Scene::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    return scenePrims->intersect(ray, isect);
}

bool Scene::intersect_p(const Ray &ray) const {
    return scenePrims->intersect_p(ray);
}
} // namespace filianore