#ifndef _AABB_H
#define _AABB_H

#include "ray.h"

namespace filianore
{
	struct AABB
	{
		StaticArray<float, 3> pMin, pMax;

		AABB() = default;

		FILIANORE_INLINE AABB(const StaticArray<float, 3> &p) : pMin(p), pMax(p) {}

		FILIANORE_INLINE AABB(const StaticArray<float, 3> &_min, const StaticArray<float, 3> &_max)
			: pMin(_min), pMax(_max)
		{
		}

		FILIANORE_INLINE static AABB Full()
		{
			float maxF = std::numeric_limits<float>::max();

			return AABB(
				StaticArray<float, 3>(-maxF, -maxF, -maxF),
				StaticArray<float, 3>(maxF, maxF, maxF));
		}

		FILIANORE_INLINE static AABB Empty()
		{
			float maxF = std::numeric_limits<float>::max();

			return AABB(
				StaticArray<float, 3>(maxF, maxF, maxF),
				StaticArray<float, 3>(-maxF, -maxF, -maxF));
		}

		FILIANORE_INLINE void Shrink(const AABB &box)
		{
			pMin.params[0] = std::max(pMin.x(), box.pMin.x());
			pMin.params[1] = std::max(pMin.y(), box.pMin.y());
			pMin.params[2] = std::max(pMin.z(), box.pMin.z());

			pMax.params[0] = std::min(pMax.x(), box.pMax.x());
			pMax.params[1] = std::min(pMax.y(), box.pMax.y());
			pMax.params[2] = std::min(pMax.z(), box.pMax.z());
		}

		FILIANORE_INLINE void Extend(const StaticArray<float, 3> &p)
		{
			pMin.params[0] = std::min(pMin.x(), p.x());
			pMin.params[1] = std::min(pMin.y(), p.y());
			pMin.params[2] = std::min(pMin.z(), p.z());

			pMax.params[0] = std::max(pMax.x(), p.x());
			pMax.params[1] = std::max(pMax.y(), p.y());
			pMax.params[2] = std::max(pMax.z(), p.z());
		}

		FILIANORE_INLINE void Extend(const AABB &box)
		{
			pMin.params[0] = std::min(pMin.x(), box.pMin.x());
			pMin.params[1] = std::min(pMin.y(), box.pMin.y());
			pMin.params[2] = std::min(pMin.z(), box.pMin.z());

			pMax.params[0] = std::max(pMax.x(), box.pMax.x());
			pMax.params[1] = std::max(pMax.y(), box.pMax.y());
			pMax.params[2] = std::max(pMax.z(), box.pMax.z());
		}

		FILIANORE_INLINE StaticArray<float, 3> Diagonal() const
		{
			return pMax - pMin;
		}

		FILIANORE_INLINE StaticArray<float, 3> Center() const
		{
			return (pMax + pMin) * 0.5f;
		}

		FILIANORE_INLINE float SurfaceArea() const
		{
			StaticArray<float, 3> d = Diagonal();
			return (d.x() * d.y() + d.x() * d.z() + d.y() * d.z()) * 2.f;
		}

		FILIANORE_INLINE float HalfArea() const
		{
			StaticArray<float, 3> d = Diagonal();
			return (d.x() + d.y()) * d.z() + d.x() * d.y();
		}

		FILIANORE_INLINE float Volume() const
		{
			StaticArray<float, 3> d = Diagonal();
			return d.x() * d.y() * d.z();
		}

		FILIANORE_INLINE int LargestAxis() const
		{
			StaticArray<float, 3> d = Diagonal();
			int axis = 0;
			if (d.x() < d.y())
				axis = 1;
			if (d.y() < d.z())
				axis = 2;
			return axis;
		}

		FILIANORE_INLINE int LargestExtent() const
		{
			return Diagonal().params[LargestAxis()];
		}

		FILIANORE_INLINE StaticArray<float, 3> Offset(const StaticArray<float, 3> &p) const
		{
			StaticArray<float, 3> o = p - pMin;
			if (pMax.x() > pMin.x())
				o.params[0] /= pMax.x() - pMin.x();
			if (pMax.y() > pMin.y())
				o.params[1] /= pMax.y() - pMin.y();
			if (pMax.z() > pMin.z())
				o.params[2] /= pMax.z() - pMin.z();
			return o;
		}

		FILIANORE_INLINE bool Intersect(const Ray &ray, float *hitt0, float *hitt1) const
		{
			StaticArray<float, 3> invRayDir = StaticArray<float, 3>(1.f) / ray.dir;
			StaticArray<float, 3> tbot = invRayDir * (pMin - ray.origin);
			StaticArray<float, 3> ttop = invRayDir * (pMax - ray.origin);

			StaticArray<float, 3> tmin = VecMin(ttop, tbot);
			StaticArray<float, 3> tmax = VecMax(ttop, tbot);

			*hitt0 = std::max(std::max(tmin.x(), tmin.y()), tmin.z());
			*hitt1 = std::min(std::min(tmax.x(), tmax.y()), tmax.z());

			return !(*hitt0 > *hitt1) && *hitt1 > 0;
		}
	};
} // namespace filianore

#endif