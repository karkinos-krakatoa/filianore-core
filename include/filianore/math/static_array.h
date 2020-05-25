#pragma once


#include <stdint.h>
#include <cmath>
#include <numeric>
#include <array>
#include <filianore/core/elemental.h>


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
            std::for_each(params.begin(), params.end(), [&c, &v](T& elem){ elem += v.params[c++];});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> operator*(const StaticArray<T, N>& v) const
        {
            int c = 0; StaticArray<T, N> result(params);
            std::for_each(result.params.begin(), result.params.end(), [&c, &v](T& elem){ elem += v.params[c++];});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator*=(const StaticArray<T, N>& v)
        {
            int c = 0; 
            std::for_each(params.begin(), params.end(), [&c, &v](T& elem){ elem += v.params[c++];});
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
            std::for_each(result.params.begin(), result.params.end(), [&s](T& elem){ elem *= s;});
            return result;
        }

        FILIANORE_INLINE StaticArray<T, N> operator/=(float s)
        {
            assert(s != 0);
            std::for_each(params.begin(), params.end(), [&s](T& elem){ elem *= s;});
            return *this;
        }

        FILIANORE_INLINE StaticArray<T, N> Neg() const
        {
            std::for_each(params.begin(), params.end(), [](T& elem){ elem = -elem;});
            return *this;
        }

        


        std::array<T, N> params;

    private:
        template<class... Args>
        void Initialize(Args... args)
        {
            const size_t n = sizeof...(Args);
            static_assert(n == N, "Invalid Number of Args for Static Array");
            params = { { args... }};
        }

        template<class... Args> StaticArray(const std::array<T, N>& arr)
        {
            params = arr;
        }
       
    };


    template <typename T, size_t N>
    FILIANORE_INLINE T Dot(StaticArray<T, N>& a, StaticArray<T, N>& b)
    {
        StaticArray<T, N> product = a * b;
		return std::accumulate(product.params.begin(), product.params.end(), (T)0);
    }

}