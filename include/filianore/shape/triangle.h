#pragma once


#include <filianore/core/shape.h>
#include <filianore/math/aabb.h>


namespace filianore
{

    template <typename T> struct TriangleEntity
	{
		TriangleEntity()
			: vertex(StaticArray<T, 3>()), normal(StaticArray<T, 3>()), normalFound(false), uv(StaticArray<T, 2>())
		{ }

		TriangleEntity(const StaticArray<T, 3>& _v, const StaticArray<T, 3>& _n, bool _normalFound = false, const StaticArray<T, 2>& _uv = StaticArray<T, 2>())
			: vertex(_v), normal(_n), normalFound(_normalFound), uv(_uv)
		{ }

		StaticArray<T, 3> vertex;
		StaticArray<T, 3> normal;
		bool normalFound;
		StaticArray<T, 2> uv;
	};


    template <typename T> class Triangle : public Shape<T>
	{
	public:
        Triangle(){ }
        
        ~Triangle(){ }

		Triangle(const TriangleEntity<T>& _v1, const TriangleEntity<T>& _v2, const TriangleEntity<T>& _v3, bool _reverseOrientation = false)
            : Shape<T>(_reverseOrientation), v1(_v1), v2(_v2), v3(_v3)
	    {
		    allNormalsInMesh = false;
		    if (_v1.normalFound && _v2.normalFound && _v3.normalFound)
		    {
			    allNormalsInMesh = true;
		    }
	    }


        AABB<T> WorldBound() const;

		bool Intersect(const Ray<T, 3>& ray, T* t) const;

		StaticArray<T, 3> Centroid() const;

		T Area() const;


	private:
		StaticArray<T, 3> GeometricNormal(const StaticArray<T, 3>& _p) const;
        StaticArray<T, 3> ShadingNormal(const StaticArray<T, 3>& _p) const;

		TriangleEntity<T> v1, v2, v3;
		bool allNormalsInMesh;
	};

}