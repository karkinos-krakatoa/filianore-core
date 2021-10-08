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
		StaticArray<float, 3> tangent;
		bool normalFound, tangentFound;
		StaticArray<float, 2> uv;

		TriangleEntity()
			: vertex(StaticArray<float, 3>()), normal(StaticArray<float, 3>()), tangent(StaticArray<float, 3>()), normalFound(false), uv(StaticArray<float, 2>())
		{
		}

		TriangleEntity(const StaticArray<float, 3> &_v, const StaticArray<float, 3> &_n = StaticArray<float, 3>(), bool _normalFound = false,
					   const StaticArray<float, 3> &_t = StaticArray<float, 3>(), bool _tangentFound = false,
					   const StaticArray<float, 2> &_uv = StaticArray<float, 2>())
			: vertex(_v), normal(_n), normalFound(_normalFound), tangent(_t), tangentFound(_tangentFound), uv(_uv)
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
			allNormalsInMesh = (v1.normalFound && v2.normalFound && v3.normalFound) ? true : false;
			allTangentsInMesh = (v1.tangentFound && v2.tangentFound && v3.tangentFound) ? true : false;
		}

		AABB WorldBound() const;

		bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;

		StaticArray<float, 3> Centroid() const;

		float Area() const;

		using Shape::Sample;
		Interaction Sample(const StaticArray<float, 2> &u, float *pdf) const;

	private:
		TriangleEntity v1, v2, v3;
		bool allNormalsInMesh;
		bool allTangentsInMesh;
	};

} // namespace filianore

#endif