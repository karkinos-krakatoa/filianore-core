#pragma once


#include "elemental.h"
#include "math/transform.h"


namespace filianore
{

	class Shape
	{
	public:
		virtual ~Shape() { }

		Shape() : reverseOrientation(false){ }

		Shape(bool _reverseOrientation) : reverseOrientation(_reverseOrientation){ }

		virtual AABB WorldBound() const = 0;

		virtual bool Intersect(const Ray& ray, float* t) const = 0;

		virtual bool IntersectP(const Ray& ray) const
		{
			return Intersect(ray, 0);
		}

		virtual StaticArray<float, 3> Centroid() const = 0;

		virtual float Area() const = 0;


		const bool reverseOrientation;
	};

}