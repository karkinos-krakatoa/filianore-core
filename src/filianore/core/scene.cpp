#include "scene.h"
#include "../illuminants/illuminant.h"
#include "primitive.h"

namespace filianore {
Scene::Scene(const BVH &_bvh, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
    : bvh(_bvh), illuminants(_illuminants) {
    for (const auto &illuminant : illuminants) {
        illuminant->prepare_illuminant(*this);
    }
}

bool Scene::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    return bvh.intersect(ray, isect);
}

bool Scene::intersect_p(const Ray &ray) const {
    return false; // bvh->intersect_p(ray);
}
} // namespace filianore