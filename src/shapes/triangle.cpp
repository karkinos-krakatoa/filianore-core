#include <filianore/shape/triangle.h>


namespace filianore
{

	template <typename T> AABB<T> Triangle<T>::WorldBound() const
    {
        AABB<T> box(v1.vertex);
		box.Union(v2.vertex);
		box.Union(v3.vertex);
		return box;
    }


	template <typename T> bool Triangle<T>::Intersect(const Ray<T, 3>& ray, T* t) const
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


	template <typename T> StaticArray<T, 3> Triangle<T>::Centroid() const
    {
        return (v1.vertex + v2.vertex + v3.vertex) / (T)3;
    }


	template <typename T> T Triangle<T>::Area() const
    {
        return (T)0.5 * Cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).Length();
    }


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


	template AABB<float> Triangle<float>::WorldBound() const;
	template bool Triangle<float>::Intersect(const Ray<float, 3>& ray, float* t) const;
	template StaticArray<float, 3> Triangle<float>::Centroid() const;
	template float Triangle<float>::Area() const;
    template StaticArray<float, 3> Triangle<float>::GeometricNormal(const StaticArray<float, 3>& _p) const;
    template StaticArray<float, 3> Triangle<float>::ShadingNormal(const StaticArray<float, 3>& _p) const;

}