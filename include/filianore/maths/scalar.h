#ifndef _SCALAR_H
#define _SCALAR_H

#include "../core/elemental.h"
#include <stdint.h>
#include <cmath>
#include <numeric>

namespace filianore
{

	template <typename T>
	constexpr T Pi = static_cast<T>(3.14159265358979323846);
	template <typename T>
	constexpr T InvPi = static_cast<T>(0.31830988618379067154);
	template <typename T>
	constexpr T Inv2Pi = static_cast<T>(0.15915494309189533577);
	template <typename T>
	constexpr T PiOver2 = static_cast<T>(1.57079632679489661923);
	template <typename T>
	constexpr T PiOver4 = static_cast<T>(0.78539816339744830961);
	template <typename T>
	constexpr T Epsilon = static_cast<T>(0.0001);

	template <typename T>
	FILIANORE_INLINE bool IsNaN(const T x)
	{
		return isnan(x);
	}

	template <typename T>
	FILIANORE_INLINE T MaxScalar()
	{
		return std::numeric_limits<T>::max();
	}

	template <typename T>
	FILIANORE_INLINE T Infinity()
	{
		return std::numeric_limits<T>::infinity();
	}

	template <typename T>
	FILIANORE_INLINE T MachineEpsilon()
	{
		return std::numeric_limits<T>::epsilon() * static_cast<T>(0.5);
	}

	template <typename T>
	FILIANORE_INLINE T Gamma(int n)
	{
		return (n * MachineEpsilon<T>()) / (1 - n * MachineEpsilon<T>());
	}

	template <typename T>
	FILIANORE_INLINE T Radians(T degrees)
	{
		return (Pi<T> / static_cast<T>(180)) * degrees;
	}

	template <typename T>
	FILIANORE_INLINE T Degrees(T radians)
	{
		return (static_cast<T>(180) / Pi<T>)*radians;
	}

	template <typename T>
	FILIANORE_INLINE T Lerp(T t, const T &input1, const T &input2)
	{
		return (static_cast<T>(1) - t) * input1 + t * input2;
	}

	template <typename T>
	FILIANORE_INLINE T Clamp(const T &val, const T &low, const T &high)
	{
		if (val < low)
		{
			return low;
		}
		else if (val > high)
		{
			return high;
		}
		else
		{
			return val;
		}
	}

	template <typename P>
	int FindInterval(int size, const P &predicate)
	{
		int first = 0, len = size;

		while (len > 0)
		{
			int half = len >> 1;
			int middle = first + half;
			if (predicate(middle))
			{
				first = middle + 1;
				len -= half + 1;
			}
			else
			{
				len = half;
			}
		}

		return Clamp<int>(first - 1, 0, size - 2);
	}

} // namespace filianore

#endif