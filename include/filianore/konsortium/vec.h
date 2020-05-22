#pragma once

#include <stdint.h>
#include <cmath>
#include <assert.h>
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

        FILIANORE_INLINE Vec3<T> operator*(const Vec3<T>& v) const
        {
            return Vec3<T>(x * v.x, y * v.y, z * v.z);
        }

        FILIANORE_INLINE Vec3<T> &operator*=(const Vec3<T>& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        FILIANORE_INLINE Vec3<T> operator*(const T& s) const
        {
            return Vec3<T>(x * s, y * s, z * s);
        }

        FILIANORE_INLINE Vec3<T> &operator*=(const T& s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        
        FILIANORE_INLINE Vec3<T> operator/(const Vec3<T>& v) const
        {
            assert(v.x != 0);
            assert(v.y != 0);
            assert(v.y != 0);
            return Vec3<T>(x / v.x, y / v.y, z / v.z);
        }

        FILIANORE_INLINE Vec3<T> &operator/=(const Vec3<T>& v)
        {
            assert(v.x != 0);
            assert(v.y != 0);
            assert(v.y != 0);
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        FILIANORE_INLINE Vec3<T> operator/(const T& s) const
        {
            assert(s != 0);
            return Vec3<T>(x / s, y / s, z / s);
        }

        FILIANORE_INLINE Vec3<T> &operator/=(const T& s)
        {
            assert(s != 0);
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        FILIANORE_INLINE T LengthSquared() const { return x * x + y * y + z * z; }
        FILIANORE_INLINE T AbsLengthSquared() const { return std::abs(LengthSquared()); }
        FILIANORE_INLINE T Length() const { return std::sqrt(LengthSquared()); }
        FILIANORE_INLINE T AbsLength() const { return std::abs(Length()); }

        FILIANORE_INLINE Vec3<T> Reverse() const
        {
            return Vec3<T>(-x, -y, -z);
        }

        FILIANORE_INLINE Vec3<T> Normalize() const
        {
            return Vec3<T>(-x, -y, -z);
        }

        FILIANORE_INLINE Vec3<T> Abs() const
        {
            return Vec3<T>(std::abs(x), std::abs(y), std::abs(z));
        }


        T x, y, z;
    };


    template <typename T>
	FILIANORE_INLINE Vec3<T> Normalize(const Vec3<T>& v)
	{
		return v / v.Length();
	}

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

    template <typename T>
	FILIANORE_INLINE T AbsDot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return std::abs(Dot(a,b));
	}


}