#ifndef _BVH_H
#define _BVH_H

#include "../maths/geometry.h"
#include <embree4/rtcore.h>

namespace filianore {

struct BasicTriangle {
    Vector3f v1, v2, v3;
};

struct TriangleMesh {
    std::vector<BasicTriangle> triangles;
};

struct SceneGeometry {
    std::vector<TriangleMesh> triangleMeshes;
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

    void add_triangle_mesh(const BasicTriangle &tri);

    void finalise();

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool intersect_p(const Ray &ray) const;

private:
    RTCDevice device;
    RTCScene scene;
};

} // namespace filianore

#endif