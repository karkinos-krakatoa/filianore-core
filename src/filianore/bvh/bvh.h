#ifndef _BVH_H
#define _BVH_H

#include "../maths/geometry.h"
#include <embree4/rtcore.h>

namespace filianore {

struct SceneGeometryTriangleMesh {
    std::vector<Vector3f> vertices;
    std::vector<Vector3ui> indices;
};

struct SceneGeometry {
    std::vector<SceneGeometryTriangleMesh> triangleMeshes;
};

class BVH {
public:
    BVH() {
        device = rtcNewDevice(NULL);
        scene = rtcNewScene(device);
    }

    ~BVH() {
        rtcReleaseScene(scene);
        rtcReleaseDevice(device);
    }

    void initialize_scene_geometry(const SceneGeometry &sceneGeometry);

    void add_triangle_mesh(const std::vector<Vector3f> &vertices,
                           const std::vector<Vector3ui> &indices);

    void finalise();

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool intersect_p(const Ray &ray) const;

private:
    RTCDevice device;
    RTCScene scene;
};

} // namespace filianore

#endif