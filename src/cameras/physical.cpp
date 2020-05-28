#include <filianore/camera/physical.h>
#include <filianore/math/ray.h>


namespace filianore
{

	template <typename T, size_t N> void PhysicalCamera<T, N>::IntializeCamera(const StaticArray<T, N>& _camPosition, const StaticArray<T, N>& camLookAt, int xRes,
			int yRes, T vFOV, T aperture, T _t0, T _t1, const StaticArray<T, N>& camUp, bool flipCameraDirection)
    {
        T focusDistance = (_camPosition - camLookAt).Length();

		lensRadius = aperture / (T)2;

		T theta = vFOV * Pi<T> / (T)180;
		T halfHeight = tanf(theta / (T)2);
		T aspect = (T)xRes / (T)yRes;
		T halfWidth = aspect * halfHeight;

		origin = camPosition;
		w = (camPosition - camLookAt).Normalize();

		if (flipCameraDirection)
		{
			// For some reason Autodesk Maya wants this...
			w = w * -(T)1;
		}
		u = (Cross(camUp, w)).Normalize();
		v = Cross(w, u);

		// Flip Back
		w = w * -(T)1;

		lowerLeftCorner = origin - (u * halfWidth * focusDistance) - (v * focusDistance * halfHeight) - (w * focusDistance);
		horizontalVec = u * (T)2 * halfWidth * focusDistance;
		verticalVec = v * (T)2 * halfHeight * focusDistance;
    }


	template <typename T, size_t N> Ray<T, N> PhysicalCamera<T, N>::AwakenRay(const StaticArray<T, 2>& _cameraSample) const
    {
        StaticArray<T, N> rd = RandomPointOnLens() * lensRadius;
		StaticArray<T, N> offset = u * rd.x() + v * rd.y();

		T time = time0 + _cameraSample.x() * (time1 - time0);

		StaticArray<T, N> orign = origin + offset;
		StaticArray<T, N> dir = lowerLeftCorner + horizontalVec * _cameraSample.x() + verticalVec * _cameraSample.y() - origin - offset;

		return Ray<T, N>(orign, dir.Normalize(), time);
    }


    template <typename T, size_t N> StaticArray<T, N> PhysicalCamera<T, N>::RandomPointOnLens() const
	{
		StaticArray<T, 3> p;
		do
		{
			p = StaticArray<T, 3>(GenericRandNumber<T>(), GenericRandNumber<T>(), (T)0) * (T)2 - StaticArray<T, 3>((T)1, (T)1, (T)0);
		} while (Dot(p, p) >= (T)1);

		return p;
	}


	template void PhysicalCamera<float, 3>::IntializeCamera(const StaticArray<float, 3>& _camPosition, const StaticArray<float, 3>& camLookAt, int xRes,
			int yRes, float vFOV, float aperture, float _t0, float _t1, const StaticArray<float, 3>& camUp, bool flipCameraDirection);
	template Ray<float, 3> PhysicalCamera<float, 3>::AwakenRay(const StaticArray<float, 2>& _cameraSample) const;
    template StaticArray<float, 3> PhysicalCamera<float, 3>::RandomPointOnLens() const;

}