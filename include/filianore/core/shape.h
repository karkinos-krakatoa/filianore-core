#pragma once


#include "../core/elemental.h"
#include "../math/transform.h"


namespace filianore
{

	template <typename T, size_t N> class Shape
	{
	public:
		virtual ~Shape() { }

		Shape() : reverseOrientation(false){ }

		Shape(bool _reverseOrientation) : reverseOrientation(_reverseOrientation){ }

		virtual AABB<T> WorldBound() const = 0;

		virtual bool Intersect(const Ray<T, N>& ray, T* t) const = 0;

		virtual bool IntersectP(const Ray<T, N>& ray) const
		{
			return Intersect(ray, 0);
		}

		virtual StaticArray<T, N> Centroid() const = 0;

		virtual T Area() const = 0;


		const bool reverseOrientation;
	};

}