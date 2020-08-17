#ifndef _SCALAR_H
#define _SCALAR_H

#include <stdint.h>
#include <cmath>
#include <numeric>

namespace filianore
{

	template <typename T>
	constexpr T Pi = static_cast<T>(3.14159265358979323846);
	template <typename T>
	constexpr T Epsilon = static_cast<T>(0.0001);

	template <typename T>
	inline bool IsNaN(const T x)
	{
		return isnan(x);
	}

	template <>
	inline bool IsNaN(const int x)
	{
		return false;
	}

	template <typename T>
	inline T MaxScalar()
	{
		return std::numeric_limits<T>::max();
	}

	template <typename T>
	inline T Infinity()
	{
		return std::numeric_limits<T>::infinity();
	}

	template <typename T>
	inline T MachineEpsilon()
	{
		return std::numeric_limits<T>::epsilon() * static_cast<T>(0.5);
	}

	template <typename T>
	inline T Gamma(int n)
	{
		return (n * MachineEpsilon<T>()) / (1 - n * MachineEpsilon<T>());
	}

	template <typename T>
	inline T Radians(T degrees)
	{
		return (Pi<T> / static_cast<T>(180)) * degrees;
	}

	template <typename T>
	inline T Degrees(T radians)
	{
		return (static_cast<T>(180) / Pi<T>)*radians;
	}

	template <typename T>
	inline T Lerp(T t, const T &input1, const T &input2)
	{
		return (static_cast<T>(1) - t) * input1 + t * input2;
	}

	template <typename T>
	inline T Clamp(const T &val, const T &low, const T &high)
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

	template <typename T>
	inline T BalanceHeuristic(int nf, T fPdf, int ng, T gPdf)
	{
		return (nf * fPdf) / (nf * fPdf + ng * gPdf);
	}

	template <typename T>
	inline T PowerHeuristic(int nf, T fPdf, int ng, T gPdf)
	{
		T f = nf * fPdf;
		T g = ng * gPdf;

		return (f * f) / (f * f + g * g);
	}

	template <typename T>
	inline T GenericRandNumber()
	{
		return (T)rand() / ((T)RAND_MAX);
	}

} // namespace filianore

#endif