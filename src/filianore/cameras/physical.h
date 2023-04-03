#ifndef _PHYSICAL_CAMERA_H
#define _PHYSICAL_CAMERA_H

#include "camera.h"

namespace filianore {
class PhysicalCamera : public Camera {
public:
    PhysicalCamera() {}

    PhysicalCamera(const Vector3f &_camPosition, const Vector3f &camLookAt,
                   int xRes, int yRes,
                   float vFOV, float aperture,
                   float _t0, float _t1,
                   const Vector3f &camUp = Vector3f(0.f, 1.f, 0.f),
                   bool flipCameraDirection = false)
        : camPosition(_camPosition), time0(_t0), time1(_t1) {
        float focusDistance = (_camPosition - camLookAt).length();

        lensRadius = aperture / 2.f;

        float theta = vFOV * PI / 180.f;
        float halfHeight = tanf(theta / 2.f);
        float aspect = (float)xRes / (float)yRes;
        float halfWidth = aspect * halfHeight;

        origin = camPosition;
        w = normalize(camPosition - camLookAt);

        if (flipCameraDirection) {
            // For some reason Maya wants this...
            w = w * -1.f;
        }
        u = normalize(cross(camUp, w));
        v = cross(w, u);

        // Flip Back
        w = w * -1.f;

        lowerLeftCorner = (Vector3f)origin - (u * halfWidth * focusDistance) - (v * focusDistance * halfHeight) - (w * focusDistance);
        horizontalVec = u * 2.f * halfWidth * focusDistance;
        verticalVec = v * 2.f * halfHeight * focusDistance;
    }

    Ray awaken_ray(const Vector2f &_cameraSample, const Vector2f &_lensSample) const;

private:
    Vector3f random_point_on_lens(const Vector2f &_lensSample) const;

    Vector3f camPosition;
    Vector3f lowerLeftCorner, horizontalVec, verticalVec;
    Vector3f origin;
    Vector3f u, v, w;

    float lensRadius;
    float time0, time1;
};
} // namespace filianore

#endif