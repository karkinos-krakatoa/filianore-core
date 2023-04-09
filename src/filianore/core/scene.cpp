#include "scene.h"
#include "../illuminants/illuminant.h"
#include "primitive.h"

namespace filianore {

Scene::Scene(const SceneGeometry &sceneGeometry, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
    : illuminants(_illuminants) {
    // Initialize BVH with Scene Geometry
    bvh = BVH();
    // bvh.initialize_scene_geometry(sceneGeometry);

    // Initialize and prep illuminants
    if (!illuminants.empty()) {
        for (const auto &illuminant : illuminants) {
            illuminant->prepare_illuminant(*this);
        }
    }
}

bool Scene::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    return bvh.intersect(ray, isect);
}

bool Scene::intersect_p(const Ray &ray) const {
    return bvh.intersect_p(ray);
}

} // namespace filianore