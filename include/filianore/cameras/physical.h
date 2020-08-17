#ifndef _PHYSICAL_CAMERA_H
#define _PHYSICAL_CAMERA_H

#include "../core/camera.h"

namespace filianore
{
    class PhysicalCamera : public Camera
    {
    public:
        PhysicalCamera() {}

        PhysicalCamera(const StaticArray<float, 3> &_camPosition, const StaticArray<float, 3> &camLookAt,
                       int xRes, int yRes,
                       float vFOV, float aperture,
                       float _t0, float _t1,
                       const StaticArray<float, 3> &camUp = StaticArray<float, 3>(0.f, 1.f, 0.f),
                       bool flipCameraDirection = false)
            : camPosition(_camPosition), time0(_t0), time1(_t1)
        {
            float focusDistance = (_camPosition - camLookAt).Length();

            lensRadius = aperture / 2.f;

            float theta = vFOV * Pi<float> / 180.f;
            float halfHeight = tanf(theta / 2.f);
            float aspect = (float)xRes / (float)yRes;
            float halfWidth = aspect * halfHeight;

            origin = camPosition;
            w = (camPosition - camLookAt).Normalize();

            if (flipCameraDirection)
            {
                // For some reason Maya wants this...
                w = w * -1.f;
            }
            u = (Cross(camUp, w)).Normalize();
            v = Cross(w, u);

            // Flip Back
            w = w * -1.f;

            lowerLeftCorner = origin - (u * halfWidth * focusDistance) - (v * focusDistance * halfHeight) - (w * focusDistance);
            horizontalVec = u * 2.f * halfWidth * focusDistance;
            verticalVec = v * 2.f * halfHeight * focusDistance;
        }

        Ray AwakenRay(const StaticArray<float, 2> &_cameraSample, const StaticArray<float, 2> &_lensSample) const;

    private:
        StaticArray<float, 3> RandomPointOnLens(const StaticArray<float, 2> &_lensSample) const;

        StaticArray<float, 3> camPosition;
        StaticArray<float, 3> lowerLeftCorner, horizontalVec, verticalVec;
        StaticArray<float, 3> origin;
        StaticArray<float, 3> u, v, w;

        float lensRadius;
        float time0, time1;
    };
} // namespace filianore

#endif