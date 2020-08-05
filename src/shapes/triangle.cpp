#include "filianore/shapes/triangle.h"


namespace filianore
{

	AABB Triangle::WorldBound() const
	{
		AABB box(v1.vertex);
		box.Extend(v2.vertex);
		box.Extend(v3.vertex);
		return box;
	}


	bool Triangle::Intersect(const Ray& ray, float* t) const
	{
		StaticArray<float, 3> e1 = v2.vertex - v1.vertex;
		StaticArray<float, 3> e2 = v3.vertex - v1.vertex;

		StaticArray<float, 3> n = Cross(e1, e2);

		float det = -Dot(ray.dir, n);

		StaticArray<float, 3> ao = ray.origin - v1.vertex;
		StaticArray<float, 3> dao = Cross(ao, ray.dir);

		float invDet = 1.f / det;

		float u = Dot(e2, dao) * invDet;
		float v = -Dot(e1, dao) * invDet;

		*t = Dot(ao, n) * invDet;

		return (*t > 0 && u > 0 && v > 0 && (u + v) < 1);
	}


	StaticArray<float, 3> Triangle::Centroid() const
	{
		return (v1.vertex + v2.vertex + v3.vertex) / 3.f;
	}


	float Triangle::Area() const
	{
		return 0.5f * Cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).Length();
	}


	StaticArray<float, 3> Triangle::GeometricNormal(const StaticArray<float, 3>& _p) const
	{
		StaticArray<float, 3> e1 = v2.vertex - v1.vertex;
		StaticArray<float, 3> e2 = v3.vertex - v1.vertex;
		StaticArray<float, 3> normal = Cross(e1, e2).Normalize();
		return this->reverseOrientation ? normal.Neg() : normal;
	}


	StaticArray<float, 3> Triangle::ShadingNormal(const StaticArray<float, 3>& _p) const
	{
		return ((v1.normal + v2.normal + v3.normal) / 3.f).Normalize();
	}


}