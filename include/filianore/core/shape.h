#ifndef _SHAPE_H
#define _SHAPE_H

#include "elemental.h"
#include "../maths/transform.h"

namespace filianore
{

	class Shape
	{
	public:
		const bool reverseOrientation;

		Shape() : reverseOrientation(false) {}

		Shape(bool _reverseOrientation) : reverseOrientation(_reverseOrientation) {}

		virtual AABB WorldBound() const = 0;

		virtual bool Intersect(const Ray &ray, SurfaceInteraction *t) const = 0;

		virtual bool IntersectP(const Ray &ray) const;

		virtual StaticArray<float, 3> Centroid() const = 0;

		virtual float Area() const = 0;

		virtual float Pdf(const Interaction &isect) const;
		virtual float Pdf(const Interaction &isect, const StaticArray<float, 3> &wi) const;

		virtual Interaction Sample(const StaticArray<float, 2> &u, float *pdf) const = 0;
		virtual Interaction Sample(const Interaction &isect, const StaticArray<float, 2> &u, float *pdf) const;
	};

} // namespace filianore

#endif