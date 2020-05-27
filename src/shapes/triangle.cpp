#include <filianore/render/triangle.h>


namespace filianore
{

    template <typename T, size_t N> AABB<T> Triangle<T, N>::WorldBound() const
	{
		AABB<T> box(v1.vertex);
		box.Union(v2.vertex);
		box.Union(v3.vertex);
		return box;
	}

    template <typename T, size_t N> bool Triangle<T, N>::Intersect(const Ray<T, N>& ray, T* t) const
	{
		*t = 0;
		return false;
	}

    template <typename T, size_t N> StaticArray<T, N> Triangle<T, N>::Centroid() const
	{
		return (v1.vertex + v2.vertex + v3.vertex) / (T)3;
	}

    template <typename T, size_t N> T Triangle<T, N>::Area() const
	{
		return (T)0.5 * Cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).Length();
	}

    template <typename T, size_t N> StaticArray<T, N> Triangle<T, N>::GeometricNormal(const StaticArray<T, N>& _p) const
	{
		StaticArray<T, N> e1 = v2.vertex - v1.vertex;
		StaticArray<T, N> e2 = v3.vertex - v1.vertex;
		StaticArray<T, N> normal = Cross(e1, e2).Normalize();
		return this->reverseOrientation ? normal.Neg() : normal;
	}

    template <typename T, size_t N> StaticArray<T, N> Triangle<T, N>::ShadingNormal(const StaticArray<T, N>& _p) const
	{
		return ((v1.normal + v2.normal + v3.normal) / (T)3).Normalize();
	}


    template AABB<float> Triangle<float, 3>::WorldBound() const;
    template bool Triangle<float, 3>::Intersect(const Ray<float, 3>& ray, float* t) const;
    template StaticArray<float, 3> Triangle<float, 3>::Centroid() const;
    template float Triangle<float, 3>::Area() const;
    template StaticArray<float, 3> Triangle<float, 3>::GeometricNormal(const StaticArray<float, 3>& _p) const;
    template StaticArray<float, 3> Triangle<float, 3>::ShadingNormal(const StaticArray<float, 3>& _p) const;
}