#ifndef _BVH_H
#define _BVH_H

#include "../maths/geometry.h"
#include <embree4/rtcore.h>

namespace filianore {

struct SceneGeometryTriangleMesh {
    std::vector<Vector3f> vertices;
    std::vector<int> indices;
};

struct SceneGeometry {
    std::vector<SceneGeometryTriangleMesh> triangleMeshes;
};

class BVH {
public:
    BVH() = default;

    void initialize() {
        device = rtcNewDevice(NULL);
        scene = rtcNewScene(device);
    }

    void clean_up() {
        rtcReleaseScene(scene);
        rtcReleaseDevice(device);
    }

    void initialize_scene_geometry(const SceneGeometry &sceneGeometry);

    void add_triangle_mesh(const std::vector<Vector3f> &vertices,
                           const std::vector<int> &indices);

    void finalise();

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool intersect_p(const Ray &ray) const;

private:
    RTCDevice device;
    RTCScene scene;
};

} // namespace filianore

#endif