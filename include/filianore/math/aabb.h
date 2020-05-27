#pragma once


#include "ray.h"


namespace filianore
{

    template <typename T> class AABB
	{
	public:
		StaticArray<T, 3> pMin, pMax, Extent;


		AABB() { }

		AABB(const  StaticArray<T, 3>& p) : pMin(p), pMax(p) { }

		AABB(const  StaticArray<T, 3>& _min, const  StaticArray<T, 3>& _max)
			: pMin(_min), pMax(_max)
		{
			Extent = pMax - pMin;
		}

		inline const  StaticArray<T, 3>& operator[](int i) const
		{
			assert(i == 0 || i == 1);
			return (i == 0) ? pMin : pMax;
		}

		inline StaticArray<T, 3>& operator[](int i)
		{
			assert(i == 0 || i == 1);
			return (i == 0) ? pMin : pMax;
		}

		inline StaticArray<T, 3> Corner(int corner) const
		{
			return  StaticArray<T, 3>((*this)[(corner & 1)].x,
				(*this)[(corner & 2) ? 1 : 0].y,
				(*this)[(corner & 4) ? 1 : 0].z);
		}

		inline StaticArray<T, 3> Diagonal() const
		{
			return pMax - pMin;
		}

		inline T SurfaceArea() const
		{
			StaticArray<T, 3> d = Diagonal();
			return (d.x * d.y + d.x * d.z + d.y * d.z) * T(2);
		}

		void Union(const  StaticArray<T, 3>& p)
		{
			pMin.x = std::min(pMin.x, p.x);
			pMin.y = std::min(pMin.y, p.y);
			pMin.z = std::min(pMin.z, p.z);

			pMax.x = std::max(pMax.x, p.x);
			pMax.y = std::max(pMax.y, p.y);
			pMax.z = std::max(pMax.z, p.z);

			Extent = pMax - pMin;
		}

		void Union(const AABB& box)
		{
			pMin.x = std::min(pMin.x, box.pMin.x);
			pMin.y = std::min(pMin.y, box.pMin.y);
			pMin.z = std::min(pMin.z, box.pMin.z);

			pMax.x = std::max(pMax.x, box.pMax.x);
			pMax.y = std::max(pMax.y, box.pMax.y);
			pMax.z = std::max(pMax.z, box.pMax.z);

			Extent = pMax - pMin;
		}

		inline T Volume() const
		{
			StaticArray<T, 3> d = Diagonal();
			return d.x * d.y * d.z;
		}

		int MaximumExtent() const
		{
			StaticArray<T, 3> d = Diagonal();
			if (d.x > d.y&& d.x > d.z)
				return 0;
			else if (d.y > d.z)
				return 1;
			else
				return 2;
		}

		StaticArray<T, 3> Offset(const StaticArray<T, 3>& p) const
		{
			StaticArray<T, 3> o = p - pMin;
			if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
			if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
			if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
			return o;
		}

		bool IntersectP(const Ray<T, 3>& ray, T* hitt0, T* hitt1) const
		{
			T t0 = ray.tMin, t1 = ray.tMax;
			for (unsigned int i = 0; i < 3; ++i)
			{
				T invRayDir = T(1) / ray.dir[i];
				T tnear = (pMin.data[i] - ray.origin[i]) * invRayDir;
				T tfar = (pMax.data[i] - ray.origin[i]) * invRayDir;

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