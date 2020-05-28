#pragma once


#include <filianore/core/camera.h>


namespace filianore
{

    template <typename T, size_t N> class PhysicalCamera : public Camera<T, N>
	{
	public:
		PhysicalCamera(){ }

        ~PhysicalCamera(){ }

		PhysicalCamera(const StaticArray<T, N>& _camPosition, const StaticArray<T, N>& camLookAt, int xRes,
			int yRes, T vFOV, T aperture, T _t0, T _t1, const StaticArray<T, N>& camUp = StaticArray<T, N>(0, 1, 0), bool flipCameraDirection = false)
        {
            IntializeCamera(_camPosition, camLookAt, xRes, yRes, vFOV, aperture, _t0, _t1, camUp, flipCameraDirection);
        }


		Ray<T, N> AwakenRay(const StaticArray<T, 2>& _cameraSample) const;


	private:
		StaticArray<T, N> RandomPointOnLens() const;
        void IntializeCamera(const StaticArray<T, N>& _camPosition, const StaticArray<T, N>& camLookAt, int xRes,
			int yRes, T vFOV, T aperture, T _t0, T _t1, const StaticArray<T, N>& camUp, bool flipCameraDirection);

		StaticArray<T, N> camPosition;
		StaticArray<T, N> lowerLeftCorner, horizontalVec, verticalVec;
		StaticArray<T, N> origin;
		StaticArray<T, N> u, v, w;

		T lensRadius;
		T time0, time1;
	};

}