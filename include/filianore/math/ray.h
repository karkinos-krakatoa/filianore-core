#pragma once


#include "vec3_math.h"
#include "scalar.h"


namespace filianore
{

    template <typename T, size_t N> class Ray
	{
	public:
	
        StaticArray<T, N> origin;
		StaticArray<T, N> dir;
		mutable T tMin;
		mutable T tMax;
		T time;


		Ray() : tMin(Epsilon<T>), tMax(Infinity<T>()), time(0) { }


		Ray(const StaticArray<T, N>& _origin, const StaticArray<T, N>& _dir, T _tMin = Epsilon<T>, T _tMax = Infinity<T>(), T _time = 0)
			: origin(_origin), dir(_dir), tMin(_tMin), tMax(_tMax), time(_time)
		{ }


		StaticArray<T, N> PointAtT(T t) const
		{
			return origin + dir * t;
		}
		
	};

}