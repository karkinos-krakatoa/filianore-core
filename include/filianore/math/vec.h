#pragma once

#include <stdint.h>
#include <math.h>

#include <filianore/core/elemental.h>


namespace filianore
{

    template <typename T> class Vec3
    {
    public:
        FILIANORE_INLINE Vec3() : x(0.f), y(0.f), z(0.f) { }

        FILIANORE_INLINE Vec3(const T& _x, const T& _y, const T& _z)
        : x(_x), y(_y), z(_z) { }

        FILIANORE_INLINE Vec3<T> operator+(const Vec3<T>& v) const
        {
            return Vec3<T>(x + v.x, y + v.y, z + v.z);
        }

        FILIANORE_INLINE Vec3<T> &operator+=(const Vec3<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        FILIANORE_INLINE Vec3<T> operator-(const Vec3<T>& v) const
        {
            return Vec3<T>(x - v.x, y - v.y, z - v.z);
        }

        FILIANORE_INLINE Vec3<T> &operator-=(const Vec3<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }
        

        T x, y, z;
    };

    template <typename T>
	FILIANORE_INLINE Vec3<T> Cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>
			((a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
				(a.x * b.y) - (a.y * b.x));
	}


    template <typename T>
	FILIANORE_INLINE T Dot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

}