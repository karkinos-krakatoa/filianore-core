#include <filianore/render/camera.h>
#include <filianore/math/ray.h>


namespace filianore
{

    template <typename T, size_t N> class PhysicalCamera : public Camera<T, N>
	{
	public:
		PhysicalCamera(){ }

		PhysicalCamera(const StaticArray<T, N>& _camPosition, const StaticArray<T, N>& camLookAt, int xRes,
			int yRes, T vFOV, T aperture, T _t0, T _t1, const StaticArray<T, N>& camUp = StaticArray<T, N>(0, 1, 0), bool flipCameraDirection = false)
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


		Ray<T, N> AwakenRay(const StaticArray<T, 2>& _cameraSample) const
        {
            StaticArray<T, N> rd = RandomPointOnLens() * lensRadius;
		    StaticArray<T, N> offset = u * rd.x() + v * rd.y();

		    T time = time0 + _cameraSample.x() * (time1 - time0);

		    StaticArray<T, N> orign = origin + offset;
		    StaticArray<T, N> dir = lowerLeftCorner + horizontalVec * _cameraSample.x() + verticalVec * _cameraSample.y() - origin - offset;

		    return Ray<T, N>(orign, dir.Normalize(), time);
        }


	private:
		StaticArray<T, N> RandomPointOnLens() const;

		StaticArray<T, N> camPosition;
		StaticArray<T, N> lowerLeftCorner, horizontalVec, verticalVec;
		StaticArray<T, N> origin;
		StaticArray<T, N> u, v, w;

		T lensRadius;
		T time0, time1;
	};


    template <typename T, size_t N> StaticArray<T, N> PhysicalCamera<T, N>::RandomPointOnLens() const
	{
		StaticArray<T, 3> p;
		do
		{
			p = StaticArray<T, 3>(GenericRandNumber<T>(), GenericRandNumber<T>(), (T)0) * (T)2 - StaticArray<T, 3>((T)1, (T)1, (T)0);
		} while (Dot(p, p) >= (T)1);

		return p;
	}

}