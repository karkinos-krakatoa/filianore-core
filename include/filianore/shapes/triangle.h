#ifndef _TRIANGLE_SHAPE_H
#define _TRIANGLE_SHAPE_H

#include "../core/shape.h"
#include "../core/aabb.h"

namespace filianore
{

	struct TriangleEntity
	{
		StaticArray<float, 3> vertex;
		StaticArray<float, 3> normal;
		bool normalFound;
		StaticArray<float, 2> uv;

		TriangleEntity()
			: vertex(StaticArray<float, 3>()), normal(StaticArray<float, 3>()), normalFound(false), uv(StaticArray<float, 2>())
		{
		}

		TriangleEntity(const StaticArray<float, 3> &_v, const StaticArray<float, 3> &_n, bool _normalFound = false, const StaticArray<float, 2> &_uv = StaticArray<float, 2>())
			: vertex(_v), normal(_n), normalFound(_normalFound), uv(_uv)
		{
		}
	};

	class Triangle : public Shape
	{
	public:
		Triangle() {}

		Triangle(const TriangleEntity &_v1, const TriangleEntity &_v2, const TriangleEntity &_v3, bool _reverseOrientation = false)
			: Shape(_reverseOrientation), v1(_v1), v2(_v2), v3(_v3)
		{
			allNormalsInMesh = false;
			if (_v1.normalFound && _v2.normalFound && _v3.normalFound)
			{
				allNormalsInMesh = true;
			}
		}

		AABB WorldBound() const;

		bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;

		StaticArray<float, 3> Centroid() const;

		float Area() const;

	private:
		StaticArray<float, 3> GeometricNormal(const StaticArray<float, 3> &_p) const;
		StaticArray<float, 3> ShadingNormal(float u, float v) const;

		TriangleEntity v1, v2, v3;
		bool allNormalsInMesh;
	};

} // namespace filianore

#endif