#ifndef _CAMERA_H
#define _CAMERA_H

#include "../maths/affine.h"
#include "../maths/geometry.h"

namespace filianore {
class Camera {
public:
    Camera() {}

    virtual Ray awaken_ray(const Vector2f &_cameraSample, const Vector2f &_lensSample) const = 0;
};
} // namespace filianore

#endif