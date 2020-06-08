#pragma once


#include "vec3_math.h"
#include "scalar.h"


namespace filianore
{

    class Ray
	{
	public:
	
        StaticArray<float, 3> origin;
		StaticArray<float, 3> dir;
		mutable float tMin;
		mutable float tMax;
		float time;


		Ray() : tMin(Epsilon<float>), tMax(Infinity<float>()), time(0) { }


		Ray(const StaticArray<float, 3>& _origin, const StaticArray<float, 3>& _dir, float _tMin = Epsilon<float>, float _tMax = Infinity<float>(), float _time = 0)
			: origin(_origin), dir(_dir), tMin(_tMin), tMax(_tMax), time(_time)
		{ }


		StaticArray<float, 3> PointAtT(float t) const
		{
			return origin + dir * t;
		}
		
	};

}