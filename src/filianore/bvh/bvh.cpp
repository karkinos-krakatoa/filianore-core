#include "bvh.h"
#include "../core/interaction.h"
#include "../maths/mathutils.h"

namespace filianore {

void BVH::initialize_scene_geometry(const SceneGeometry &sceneGeometry) {
    bool geometryExists = false;

    if (!sceneGeometry.triangleMeshes.empty()) {
        geometryExists = true;
        for (auto triangleMesh : sceneGeometry.triangleMeshes) {
            for (auto tri : triangleMesh.triangles) {
                add_triangle_mesh(tri);
            }
        }
    }

    if (geometryExists) {
        finalise();
    }
}

void BVH::add_triangle_mesh(const BasicTriangle &tri) {
    //
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    // set vertices
    Vector3f *verts = (Vector3f *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(Vector3f), 3);
    verts[0] = tri.v1;
    verts[1] = tri.v2;
    verts[2] = tri.v3;

    // set indices
    Vector3ui *triangles = (Vector3ui *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(Vector3ui), 1);
    triangles[0] = 0;
    triangles[1] = 1;
    triangles[2] = 2;

    rtcCommitGeometry(geometry);
    rtcAttachGeometry(scene, geometry);
    rtcReleaseGeometry(geometry);
}

void BVH::finalise() {
    rtcCommitScene(scene);
}

bool BVH::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    RTCRayHit rayhit;
    rayhit.ray.org_x = ray.origin[0];
    rayhit.ray.org_y = ray.origin[1];
    rayhit.ray.org_z = ray.origin[2];
    rayhit.ray.dir_x = ray.dir[0];
    rayhit.ray.dir_y = ray.dir[1];
    rayhit.ray.dir_z = ray.dir[2];
    rayhit.ray.tnear = ray.tMin;
    rayhit.ray.tfar = ray.tMax;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    RTCRayQueryContext context;
    rtcInitRayQueryContext(&context);

    rtcIntersect1(scene, &rayhit);
    if (rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID || rayhit.hit.primID == RTC_INVALID_GEOMETRY_ID) {
        return false;
    }

    Vector3f wo = ray.dir;
    *isect = SurfaceInteraction(0, Vector3f(), Vector2f(),
                                Vector3f(), Vector3f(), Vector3f(), Vector3f(),
                                -ray.dir, NULL, 0);

    auto primId = rayhit.hit.geomID;

    isect->t = rayhit.ray.tfar;
    isect->p = ray.point_at_t(isect->t);
    isect->pError = abs(isect->p) * gamma<int>(5);
    isect->n = normalize(Vector3f(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z));

    ray.tMax = isect->t;

    return true;
}

bool BVH::intersect_p(const Ray &ray) const {
    return false;
}

} // namespace filianore