#ifndef _STATIC_ARRAY_H
#define _STATIC_ARRAY_H


#include <stdint.h>
#include <cmath>
#include <numeric>
#include <array>
#include "core/elemental.h"


namespace filianore
{

    template<typename T, size_t N> class StaticArray
    {
    public:
        StaticArray() { }

        template<class... Args> StaticArray(Args... args)
        {
            Initialize(args...);
        }

        template<class... Args> StaticArray(const std::array<T, N>& arr)
        {
            params = arr;
        }

        FILIANORE_INLINE T x() const
	    {
		    static_assert(1 <= N, "Invalid number of arguments for vector type");
		    return params[0];
	    }

	    FILIANORE_INLINE T y() const
	    {
            static_assert(2 <= N, "Invalid number of arguments for vector type");
            return params[1];
	    }

	    FILIANORE_INLINE T z() const
	    {
            static_assert(3 <= N, "Invalid number of arguments for vector type");
            return params[2];
	    }

	    FILIANORE_INLINE T w() const
	    {
            static_assert(4 <= N, "Invalid number of arguments for vector type");
            return params[3];
	    }

        FILIANORE_INLINE StaticArray<T, N> operator+(const StaticArray<T, N>& v) const
        {
            int c = 0; StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&c, &v](T& elem){ elem += v.params[c++];});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator+=(const StaticArray<T, N>& v)
        {
            int c = 0;
            std::for_each(params.begin(), params.end(), [&c, &v](T& elem){ elem += v.params[c++];});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> operator-(const StaticArray<T, N>& v) const
        {
            int c = 0; StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&c, &v](T& elem){ elem -= v.params[c++];});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator-=(const StaticArray<T, N>& v)
        {
            int c = 0; 
            std::for_each(params.begin(), params.end(), [&c, &v](T& elem){ elem -= v.params[c++];});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> operator*(const StaticArray<T, N>& v) const
        {
            int c = 0; StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&c, &v](T& elem){ elem *= v.params[c++];});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator*=(const StaticArray<T, N>& v)
        {
            int c = 0; 
            std::for_each(params.begin(), params.end(), [&c, &v](T& elem){ elem *= v.params[c++];});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> operator*(float s) const
        {
            StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&s](T& elem){ elem *= s;});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator*=(float s)
        {
            std::for_each(params.begin(), params.end(), 
            [&s](T& elem){ elem *= s;});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> operator/(const StaticArray<T, N>& v) const
        {
            int c = 0; StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&c, &v](T& elem){ elem /= v.params[c++];});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator/=(const StaticArray<T, N>& v)
        {
            int c = 0;
            std::for_each(params.begin(), params.end(), [&c, &v](T& elem){ elem /= v.params[c++];});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> operator/(float s) const
        {
            assert(s != 0);
            StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&s](T& elem){ elem /= s;});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator/=(float s)
        {
            assert(s != 0);
            std::for_each(params.begin(), params.end(), [&s](T& elem){ elem /= s;});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> Neg()
        {
            std::for_each(params.begin(), params.end(), [](T& elem){ elem = -elem;});
            return *this;
        }

        FILIANORE_INLINE T LengthSquared() const
	    {
            std::array<T, N> data = params;
            std::for_each(data.begin(), data.end(), [](T& elem) { elem = elem * elem; });
            return (T)std::accumulate(data.begin(), data.end(), (T)0);
	    }

        FILIANORE_INLINE T Length() const
	    {
            return std::sqrt(LengthSquared());
	    }

        FILIANORE_INLINE StaticArray<T, N> Normalize()
	    {
		    T invLength = T(1) / Length();
		    std::for_each(params.begin(), params.end(), [&invLength](T& elem){ elem *= invLength;});
            return *this;
	    }

        FILIANORE_INLINE T MinComponent()
	    {
            T min = params[0];
            std::for_each(params.begin(), params.end(), [&min](T& elem) { min = std::min(min, elem); });
            return min;
	    }

        FILIANORE_INLINE T MaxComponent()
	    {
            T max = params[0];
            std::for_each(params.begin(), params.end(), [&max](T& elem) { max = std::max(max, elem); });
            return max;
	    }


        std::array<T, N> params;

    private:
        template<class... Args>
        void Initialize(Args... args)
        {
            const size_t n = sizeof...(Args);
            static_assert(n == N, "Invalid Number of Arguments for Static Array");
            params = { { args... }};
        }
       
    };


    template <typename T, size_t N>
    FILIANORE_INLINE T Dot(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
    {
        StaticArray<T, N> product = a * b;
		return std::accumulate(product.params.begin(), product.params.end(), (T)0);
    }

    template <typename T, size_t N>
    FILIANORE_INLINE T AbsDot(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
    {
        return std::abs(Dot(a, b));
    }

    template <typename T, size_t N>
    FILIANORE_INLINE StaticArray<T, N> VecMin(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
    {
        int c = 0; StaticArray<T, N> result(a.params);
        std::for_each(result.params.begin(), result.params.end(), [&result, &b, &c](T& elem) { elem = std::min(elem, b.params[c++]); });
        return result;
    }

    template <typename T, size_t N>
    FILIANORE_INLINE StaticArray<T, N> VecMax(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
    {
        int c = 0; StaticArray<T, N> result(a.params);
        std::for_each(result.params.begin(), result.params.end(), [&result, &b, &c](T& elem) { elem = std::max(elem, b.params[c++]); });
        return result;
    }

    template <typename T, size_t N>
	FILIANORE_INLINE StaticArray<T, N> ProjectAUntoB(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
	{
		assert(a.LengthSquared() != 0);
		return a * (Dot(a, b) / a.LengthSquared());
	}

    template <typename T, size_t N>
	FILIANORE_INLINE T ProjectAUntoBScalar(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
	{
		assert(a.Length() != 0);
		return Dot(a, b) / a.Length();
	}

    template <typename T, size_t N>
	FILIANORE_INLINE StaticArray<T, N> Reflect(const StaticArray<T, N>& a, const StaticArray<T, N>& b)
	{
		return a - b * ((Dot(a, b)) * T(2));
	}

    template <typename T, size_t N>
	FILIANORE_INLINE StaticArray<T, N> Faceforward(const StaticArray<T, N>& n, const StaticArray<T, N>& v)
	{
		T dot = Dot(n, v);
		if (dot != 0)
		{
			return dot > 0 ? n : -n;
		}
		StaticArray<T, N> offset = StaticArray<T, N>(T(0.0001), 0, 0);
		dot = Dot(n + offset, v);
		return dot > 0 ? n : -n;
	}

    template <typename T, size_t N>
	FILIANORE_INLINE bool Refract(const StaticArray<T, N>& wi, const StaticArray<T, N>& n, const T& eta, StaticArray<T, N>* wt)
	{
		T cosI = Dot(wi, n);
		T sinI2 = std::max(T(0), T(1) - (cosI * cosI));
		T sinT2 = eta * eta * sinI2;

		if (sinT2 >= T(1)) /* Total Internal Reflection */
		{
			return false;
		}

		T cosT = std::sqrt(1 - sinT2);
		*wt = wi * -eta + n * (eta * cosI - cosT);
		return true;
	}

    template <typename T, size_t N>
	FILIANORE_INLINE StaticArray<T, N> Permute(const StaticArray<T, N>& p, int dimens[N])
	{
		int c = 0; StaticArray<T, N> result = p;
        std::for_each(result.params.begin(), result.params.end(), [&c, &result, &dimens](T& elem) { elem = result.params[dimens[c++]]; });
		return result;
	}

    template <typename T, size_t N>
	FILIANORE_INLINE StaticArray<T, N> Lerp(T t, const StaticArray<T, N>& v0, const StaticArray<T, N>& v1)
	{
		return (T(1) - t) * v0 + t * v1;
	}

}


#endif