#ifndef _VEC3_MATH_H
#define _VEC3_MATH_H

#include "static_array.h"

namespace filianore
{

	template <typename T>
	FILIANORE_INLINE void CoordinateSystem(const StaticArray<T, 3> &v1, StaticArray<T, 3> *v2, StaticArray<T, 3> *v3)
	{
		if (std::abs(v1.x()) > std::abs(v1.y()))
		{
			T invLen = T(1) / std::sqrt(v1.x() * v1.x() + v1.z() * v1.z());
			*v2 = StaticArray<T, 3>(v1.z() * invLen, 0.f, -v1.x() * invLen);
		}
		else
		{
			T invLen = T(1) / std::sqrt(v1.y() * v1.y() + v1.z() * v1.z());
			*v2 = StaticArray<T, 3>(0.f, v1.z() * invLen, -v1.y() * invLen);
		}
		*v3 = Cross(*v2, v1);
	}

	template <typename T>
	FILIANORE_INLINE StaticArray<T, 3> Abs(const StaticArray<T, 3> &a)
	{
		return StaticArray<T, 3>(std::abs(a.params[0]), std::abs(a.params[1]), std::abs(a.params[2]));
	}

	template <typename T>
	FILIANORE_INLINE StaticArray<T, 3> Cross(const StaticArray<T, 3> &a, const StaticArray<T, 3> &b)
	{
		return StaticArray<T, 3>((a.params[1] * b.params[2]) - (a.params[2] * b.params[1]),
								 (a.params[2] * b.params[0]) - (a.params[0] * b.params[2]), (a.params[0] * b.params[1]) - (a.params[1] * b.params[0]));
	}

	template <typename T>
	FILIANORE_INLINE StaticArray<T, 3> Faceforward(const StaticArray<T, 3> &n, const StaticArray<T, 3> &v)
	{
		T dot = Dot(n, v);
		if (dot != T(0))
		{
			return dot > T(0) ? n : StaticArray<T, 3>(-n.x(), -n.y(), -n.z());
		}
		StaticArray<T, 3> offset = StaticArray<T, 3>(T(0.0001), T(0), T(0));
		dot = Dot(n + offset, v);
		return dot > T(0) ? n : StaticArray<T, 3>(-n.x(), -n.y(), -n.z());
	}

	template <typename T>
	FILIANORE_INLINE StaticArray<T, 3> Vec3Permute(const StaticArray<T, 3> &a, int x, int y, int z)
	{
		return StaticArray<T, 3>(a.params[x], a.params[y], a.params[z]);
	}

	template <typename T>
	FILIANORE_INLINE int Vec3MaxDimension(const StaticArray<T, 3> &v)
	{
		return (v.params[0] > v.params[1]) ? ((v.params[0] > v.params[2]) ? 0 : 2) : ((v.params[1] > v.params[2]) ? 1 : 2);
	}

	template <typename T>
	FILIANORE_INLINE StaticArray<T, 3> SphericalToVec3(T theta, T phi)
	{
		return StaticArray<T, 3>(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
	}

	template <typename T>
	FILIANORE_INLINE StaticArray<T, 2> Vec3ToSpherical(const StaticArray<T, 3> &a)
	{
		assert(a.params[0] != 0);
		assert(a.params[2] != 0);
		T theta = atan(a.params[1] / a.params[0]);
		T phi = atan(sqrt(a.params[0] * a.params[0] + a.params[1] * a.params[1]) / a.params[2]);
		return StaticArray<T, 2>(theta, phi);
	}

} // namespace filianore

#endif