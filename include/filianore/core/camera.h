#ifndef _CAMERA_H
#define _CAMERA_H

#include "../maths/transform.h"
#include "../maths/vec3_math.h"
#include "ray.h"

namespace filianore
{
	class Camera
	{
	public:
		virtual ~Camera() {}

		virtual Ray AwakenRay(const StaticArray<float, 2> &_cameraSample, const StaticArray<float, 2> &_lensSample) const = 0;
	};
} // namespace filianore

#endif