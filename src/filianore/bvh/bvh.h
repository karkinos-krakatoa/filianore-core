#ifndef _BVH_H
#define _BVH_H

#include "../maths/geometry.h"
#include <embree4/rtcore.h>

namespace filianore {

struct BVH {
    BVH() {
        device = rtcNewDevice(NULL);
        scene = rtcNewScene(device);
    }

    void add_triangle_mesh(uint32_t verticesCount) {
        RTCGeometry mesh = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
        Vector3f *vertices = (Vector3f *)rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(Vector3f), verticesCount);

        rtcCommitGeometry(mesh);
        rtcAttachGeometry(scene, mesh);
        rtcReleaseGeometry(mesh);
    }

    ~BVH() {
        rtcReleaseScene(scene);
        rtcReleaseDevice(device);
    }

    RTCDevice device;
    RTCScene scene;
};

} // namespace filianore

#endif