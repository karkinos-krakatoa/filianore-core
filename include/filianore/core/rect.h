#ifndef _RECT_H
#define _RECT_H

#include "ray.h"

namespace filianore
{
    struct Rect
    {
        StaticArray<float, 2> pMin, pMax;

        Rect() = default;

        FILIANORE_INLINE Rect(const StaticArray<float, 2> &p) : pMin(p), pMax(p) {}

        FILIANORE_INLINE Rect(const StaticArray<float, 2> &_min, const StaticArray<float, 2> &_max)
            : pMin(_min), pMax(_max)
        {
        }

        FILIANORE_INLINE static Rect Full()
        {
            float maxF = std::numeric_limits<float>::max();

            return Rect(
                StaticArray<float, 2>(-maxF, -maxF),
                StaticArray<float, 2>(maxF, maxF));
        }

        FILIANORE_INLINE static Rect Empty()
        {
            float maxF = std::numeric_limits<float>::max();

            return Rect(
                StaticArray<float, 2>(maxF, maxF),
                StaticArray<float, 2>(-maxF, -maxF));
        }

        FILIANORE_INLINE void Shrink(const Rect &box)
        {
            pMin.params[0] = std::max(pMin.x(), box.pMin.x());
            pMin.params[1] = std::max(pMin.y(), box.pMin.y());

            pMax.params[0] = std::min(pMax.x(), box.pMax.x());
            pMax.params[1] = std::min(pMax.y(), box.pMax.y());
        }

        FILIANORE_INLINE void Extend(const StaticArray<float, 2> &p)
        {
            pMin.params[0] = std::min(pMin.x(), p.x());
            pMin.params[1] = std::min(pMin.y(), p.y());

            pMax.params[0] = std::max(pMax.x(), p.x());
            pMax.params[1] = std::max(pMax.y(), p.y());
        }

        FILIANORE_INLINE void Extend(const Rect &box)
        {
            pMin.params[0] = std::min(pMin.x(), box.pMin.x());
            pMin.params[1] = std::min(pMin.y(), box.pMin.y());

            pMax.params[0] = std::max(pMax.x(), box.pMax.x());
            pMax.params[1] = std::max(pMax.y(), box.pMax.y());
        }

        FILIANORE_INLINE StaticArray<float, 2> Diagonal() const
        {
            return pMax - pMin;
        }

        FILIANORE_INLINE StaticArray<float, 2> Center() const
        {
            return (pMax + pMin) * 0.5f;
        }

        FILIANORE_INLINE float SurfaceArea() const
        {
            return (pMax.x() - pMin.x()) * (pMax.y() - pMin.y());
        }

        FILIANORE_INLINE int LargestAxis() const
        {
            StaticArray<float, 2> d = Diagonal();
            int axis = 0;
            if (d.x() < d.y())
                axis = 1;
            return axis;
        }

        FILIANORE_INLINE int LargestExtent() const
        {
            return Diagonal().params[LargestAxis()];
        }

        FILIANORE_INLINE StaticArray<float, 2> Offset(const StaticArray<float, 2> &p) const
        {
            StaticArray<float, 2> o = p - pMin;
            if (pMax.x() > pMin.x())
                o.params[0] /= pMax.x() - pMin.x();
            if (pMax.y() > pMin.y())
                o.params[1] /= pMax.y() - pMin.y();
            return o;
        }
    };
} // namespace filianore

#endif