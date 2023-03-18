#include "physical.h"

namespace filianore {
Vector3f PhysicalCamera::random_point_on_lens(const Vector2f &_lensSample) const {
    Vector3f p;
    do {
        p = Vector3f(_lensSample.x, _lensSample.y, 0.f) * 2.f - Vector3f(1.f, 1.f, 0.f);
    } while (dot(p, p) >= 1.f);
    return Vector3f(p.x, p.y, p.z);
}

Ray PhysicalCamera::awaken_ray(const Vector2f &_cameraSample, const Vector2f &_lensSample) const {
    Vector3f rd = random_point_on_lens(_lensSample) * lensRadius;
    Vector3f offset = u * rd.x + v * rd.y;

    float time = time0 + _cameraSample.x * (time1 - time0);

    Vector3f orign = origin + offset;
    Vector3f dir = lowerLeftCorner + horizontalVec * _cameraSample.x + verticalVec * _cameraSample.y - origin - offset;

    return Ray(orign, normalize(dir), time);
}
} // namespace filianore