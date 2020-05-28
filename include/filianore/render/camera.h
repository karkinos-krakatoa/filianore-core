#pragma once


#include "../math/transform.h"


namespace filianore
{

	template <typename T, size_t N> class Camera
	{
	public:
		virtual ~Camera() { }

		virtual Ray<T, N> AwakenRay(const StaticArray<T, 2>& _cameraSample) const = 0;
	};
    
}