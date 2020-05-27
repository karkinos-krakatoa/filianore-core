#pragma once


#include "../core/shape.h"


namespace filianore
{

    template <typename T, size_t N> struct TriangleEntity
	{
		TriangleEntity()
			: vertex(StaticArray<T, N>()), normal(StaticArray<T, N>()), normalFound(false), uv(StaticArray<T, 2>())
		{ }

		TriangleEntity(const StaticArray<T, N>& _v, const StaticArray<T, N>& _n, bool _normalFound = false, const StaticArray<T, 2>& _uv = StaticArray<T, 2>())
			: vertex(_v), normal(_n), normalFound(_normalFound), uv(_uv)
		{ }

		StaticArray<T, N> vertex;
		StaticArray<T, N> normal;
		bool normalFound;
		StaticArray<T, 2> uv;
	};


    template <typename T, size_t N> class Triangle : public Shape<T, N>
	{
	public:
		Triangle(const TriangleEntity<T, N>& _v1, const TriangleEntity<T, N>& _v2, const TriangleEntity<T, N>& _v3, bool _reverseOrientation = false)
			: Shape<T, N>(_reverseOrientation), v1(_v1), v2(_v2), v3(_v3)
		{
			allNormalsInMesh = false;
			if (_v1.normalFound && _v2.normalFound && _v3.normalFound)
			{
				allNormalsInMesh = true;
			}
		}

        AABB<T> WorldBound() const = 0;

		bool Intersect(const Ray<T, N>& ray, T* t) const = 0;

		StaticArray<T, N> Centroid() const = 0;

		T Area() const = 0;


	private:
		StaticArray<T, N> GeometricNormal(const StaticArray<T, N>& _p) const;
        StaticArray<T, N> ShadingNormal(const StaticArray<T, N>& _p) const;

		TriangleEntity<T, N> v1, v2, v3;
		bool allNormalsInMesh;
	};

}