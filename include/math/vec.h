#pragma once

#include <stdint.h>
#include <math.h>

#include <filianore/elemental.h>


namespace filianore
{

    template <typename T> class Vec2
    {
    public:
        FILIANORE_INLINE Vec2() : x(0.f), y(0.f) { }

        FILIANORE_INLINE Vec2(const T& _x, const T& _y)
        : x(_x), y(_y) { }

        FILIANORE_INLINE Vec2<T> operator+(const Vec2<T>& v) const
        {
            return Vec2<T>(x + v.x, y + v.y);
        }

        FILIANORE_INLINE Vec2<T> &operator+=(const Vec2<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        FILIANORE_INLINE Vec2<T> operator-(const Vec2<T>& v) const
        {
            return Vec2<T>(x - v.x, y - v.y);
        }

        FILIANORE_INLINE Vec2<T> &operator-=(const Vec2<T>& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        

        T x, y;
    };

}