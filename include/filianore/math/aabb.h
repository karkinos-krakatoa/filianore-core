#pragma once


#include "ray.h"


namespace filianore
{

    class AABB
	{
	public:
		StaticArray<float, 3> pMin, pMax, Extent;


		AABB() { }

		AABB(const StaticArray<float, 3>& p) : pMin(p), pMax(p) { }

		AABB(const StaticArray<float, 3>& _min, const StaticArray<float, 3>& _max)
			: pMin(_min), pMax(_max)
		{
			Extent = pMax - pMin;
		}

		FILIANORE_INLINE const StaticArray<float, 3>& operator[](int i) const
		{
			assert(i == 0 || i == 1);
			return (i == 0) ? pMin : pMax;
		}

		FILIANORE_INLINE StaticArray<float, 3>& operator[](int i)
		{
			assert(i == 0 || i == 1);
			return (i == 0) ? pMin : pMax;
		}

		FILIANORE_INLINE StaticArray<float, 3> Corner(int corner) const
		{
			return  StaticArray<float, 3>((*this)[(corner & 1)].x(),
				(*this)[(corner & 2) ? 1 : 0].y(),
				(*this)[(corner & 4) ? 1 : 0].z());
		}

		FILIANORE_INLINE StaticArray<float, 3> Diagonal() const
		{
			return pMax - pMin;
		}

		FILIANORE_INLINE float SurfaceArea() const
		{
			StaticArray<float, 3> d = Diagonal();
			return (d.x() * d.y() + d.x() * d.z() + d.y() * d.z()) * 2.f;
		}

		void Union(const  StaticArray<float, 3>& p)
		{
			pMin.params[0] = std::min(pMin.x(), p.x());
			pMin.params[1] = std::min(pMin.y(), p.y());
			pMin.params[2] = std::min(pMin.z(), p.z());

			pMax.params[0] = std::max(pMax.x(), p.x());
			pMax.params[1] = std::max(pMax.y(), p.y());
			pMax.params[2] = std::max(pMax.z(), p.z());

			Extent = pMax - pMin;
		}

		void Union(const AABB& box)
		{
			pMin.params[0] = std::min(pMin.x(), box.pMin.x());
			pMin.params[1] = std::min(pMin.y(), box.pMin.y());
			pMin.params[2] = std::min(pMin.z(), box.pMin.z());

			pMax.params[0] = std::max(pMax.x(), box.pMax.x());
			pMax.params[1] = std::max(pMax.y(), box.pMax.y());
			pMax.params[2] = std::max(pMax.z(), box.pMax.z());

			Extent = pMax - pMin;
		}

		FILIANORE_INLINE float Volume() const
		{
			StaticArray<float, 3> d = Diagonal();
			return d.x() * d.y() * d.z();
		}

		int MaximumExtent() const
		{
			StaticArray<float, 3> d = Diagonal();
			if (d.x() > d.y() && d.x() > d.z())
				return 0;
			else if (d.y() > d.z())
				return 1;
			else
				return 2;
		}

		StaticArray<float, 3> Offset(const StaticArray<float, 3>& p) const
		{
			StaticArray<float, 3> o = p - pMin;
			if (pMax.x() > pMin.x()) o.params[0] /= pMax.x() - pMin.x();
			if (pMax.y() > pMin.y()) o.params[1] /= pMax.y() - pMin.y();
			if (pMax.z() > pMin.z()) o.params[2] /= pMax.z() - pMin.z();
			return o;
		}

		bool IntersectP(const Ray& ray, float* hitt0, float* hitt1) const
		{
			float t0 = ray.tMin, t1 = ray.tMax;
			for (unsigned int i = 0; i < 3; ++i)
			{
				float invRayDir = 1.f / ray.dir.params[i];
				float tnear = (pMin.params[i] - ray.origin.params[i]) * invRayDir;
				float tfar = (pMax.params[i] - ray.origin.params[i]) * invRayDir;

				if (tnear > tfar) std::swap(tnear, tfar);
				t0 = tnear > t0 ? tnear : t0;
				t1 = tfar < t1 ? tfar : t1;

				if (t0 > t1) return false;
			}

			if (hitt0)*hitt0 = t0;
			if (hitt1)*hitt1 = t1;
			return true;
		}
	};

}