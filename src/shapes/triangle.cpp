#include <filianore/render/shape.h>
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
		Triangle(const TriangleEntity<T>& _v1, const TriangleEntity<T>& _v2, const TriangleEntity<T>& _v3, bool _reverseOrientation = false)
			: Shape<T>(_reverseOrientation), v1(_v1), v2(_v2), v3(_v3)
		{
			allNormalsInMesh = false;
			if (_v1.normalFound && _v2.normalFound && _v3.normalFound)
			{
				allNormalsInMesh = true;
			}
		}


        AABB<T> WorldBound() const
        {
            AABB<T> box(v1.vertex);
		    box.Union(v2.vertex);
		    box.Union(v3.vertex);
		    return box;
        }


		bool Intersect(const Ray<T, 3>& ray, T* t) const
        {
            StaticArray<T, 3> e1 = v2.vertex - v1.vertex;
		    StaticArray<T, 3> e2 = v3.vertex - v1.vertex;

		    StaticArray<T, 3> n = Cross(e1, e2);

		    T det = -Dot(ray.dir, n);

		    StaticArray<T, 3> ao = ray.origin - v1.vertex;
		    StaticArray<T, 3> dao = Cross(ao, ray.dir);

		    T invDet = (T)1 / det;

		    T u = Dot(e2, dao) * invDet;
		    T v = -Dot(e1, dao) * invDet;

		    *t = Dot(ao, n) * invDet;

		    return (*t > 0 && u > 0 && v > 0 && (u + v) < 1);
        }


		StaticArray<T, 3> Centroid() const
        {
            return (v1.vertex + v2.vertex + v3.vertex) / (T)3;
        }

		T Area() const
        {
            return (T)0.5 * Cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).Length();
        }


	private:
		StaticArray<T, 3> GeometricNormal(const StaticArray<T, 3>& _p) const;
        StaticArray<T, 3> ShadingNormal(const StaticArray<T, 3>& _p) const;

		TriangleEntity<T> v1, v2, v3;
		bool allNormalsInMesh;
	};


    template <typename T> StaticArray<T, 3> Triangle<T>::GeometricNormal(const StaticArray<T, 3>& _p) const
	{
		StaticArray<T, 3> e1 = v2.vertex - v1.vertex;
		StaticArray<T, 3> e2 = v3.vertex - v1.vertex;
		StaticArray<T, 3> normal = Cross(e1, e2).Normalize();
		return this->reverseOrientation ? normal.Neg() : normal;
	}

    template <typename T> StaticArray<T, 3> Triangle<T>::ShadingNormal(const StaticArray<T, 3>& _p) const
	{
		return ((v1.normal + v2.normal + v3.normal) / (T)3).Normalize();
	}

}