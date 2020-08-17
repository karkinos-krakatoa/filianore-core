#include "filianore/cameras/physical.h"

namespace filianore
{
    StaticArray<float, 3> PhysicalCamera::RandomPointOnLens(const StaticArray<float, 2> &_lensSample) const
    {
        StaticArray<float, 3> p;
        do
        {
            p = StaticArray<float, 3>(_lensSample.x(), _lensSample.y(), 0.f) * 2.f - StaticArray<float, 3>(1.f, 1.f, 0.f);
        } while (Dot(p, p) >= 1.f);
        return p;
    }

    Ray PhysicalCamera::AwakenRay(const StaticArray<float, 2> &_cameraSample, const StaticArray<float, 2> &_lensSample) const
    {
        StaticArray<float, 3> rd = RandomPointOnLens(_lensSample) * lensRadius;
        StaticArray<float, 3> offset = u * rd.x() + v * rd.y();

        float time = time0 + _cameraSample.x() * (time1 - time0);

        StaticArray<float, 3> orign = origin + offset;
        StaticArray<float, 3> dir = lowerLeftCorner + horizontalVec * _cameraSample.x() + verticalVec * _cameraSample.y() - origin - offset;

        return Ray(orign, dir.Normalize(), time);
    }
} // namespace filianore