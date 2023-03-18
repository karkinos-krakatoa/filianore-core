#ifndef _RECT_H
#define _RECT_H

#include "../maths/mathutils.h"
#include "ray.h"

namespace filianore {
template <typename T>
class Rect {
public:
    Vector2f pMin, pMax;

    Rect() = default;

    FILIANORE_INLINE Rect(const Vector2f &p) : pMin(p), pMax(p) {}

    FILIANORE_INLINE Rect(const Vector2f &_min, const Vector2f &_max)
        : pMin(_min), pMax(_max) {
    }

    FILIANORE_INLINE static Rect full() {
        T maxF = std::numeric_limits<T>::max();

        return Rect(
            Vector2f(-maxF, -maxF),
            Vector2f(maxF, maxF));
    }

    FILIANORE_INLINE static Rect empty() {
        T maxF = std::numeric_limits<T>::max();

        return Rect(
            Vector2f(maxF, maxF),
            Vector2f(-maxF, -maxF));
    }

    FILIANORE_INLINE void shrink(const Rect &box) {
        pMin = filianore::max(pMin, box.pMin);
        pMax = filianore::min(pMax, box.pMax);
    }

    FILIANORE_INLINE void extend(const Vector2f &p) {
        pMin = filianore::min(pMin, p);
        pMax = filianore::max(pMax, p);
    }

    FILIANORE_INLINE void extend(const Rect &box) {
        pMin = filianore::min(pMin, box.pMin);
        pMax = filianore::max(pMax, box.pMax);
    }

    FILIANORE_INLINE Vector2f diagonal() const {
        return pMax - pMin;
    }

    FILIANORE_INLINE Vector2f center() const {
        return (pMax + pMin) * 0.5f;
    }

    FILIANORE_INLINE T surface_area() const {
        return (pMax.x - pMin.x) * (pMax.y - pMin.y);
    }

    FILIANORE_INLINE int largest_axis() const {
        Vector2f d = diagonal();
        int axis = 0;
        if (d.x < d.y)
            axis = 1;
        return axis;
    }

    FILIANORE_INLINE int largest_extent() const {
        return diagonal()[largest_axis()];
    }

    FILIANORE_INLINE Vector2f offset(const Vector2f &p) const {
        Vector2f o = p - pMin;
        if (pMax.x > pMin.x)
            o[0] /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o[1] /= pMax.y - pMin.y;
        return o;
    }
};
} // namespace filianore

#endif