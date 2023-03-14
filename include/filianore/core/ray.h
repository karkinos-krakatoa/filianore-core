#ifndef _RAY_H
#define _RAY_H

#include "../maths/mathutils.h"
#include "../maths/vec.h"

namespace filianore {
class Ray {
public:
    Vector3f origin;
    Vector3f dir;
    mutable float tMin;
    mutable float tMax;
    float time;

    Ray() : tMin(SHADOW_EPSILON), tMax(INFINITI), time(0) {}

    Ray(const Ray &ray)
        : origin(ray.origin), dir(ray.dir), tMin(ray.tMin), tMax(ray.tMax), time(ray.time) {
    }

    Ray(const Vector3f &_origin, const Vector3f &_dir, float _tMin = SHADOW_EPSILON, float _tMax = INFINITI, float _time = 0)
        : origin(_origin), dir(_dir), tMin(_tMin), tMax(_tMax), time(_time) {
    }

    Vector3f point_at_t(float t) const {
        return origin + dir * t;
    }
};

} // namespace filianore

#endif