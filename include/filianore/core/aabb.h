#ifndef _AABB_H
#define _AABB_H

#include "ray.h"

namespace filianore {
struct AABB {
    Vector3f pMin, pMax;

    AABB() = default;

    FILIANORE_INLINE AABB(const Vector3f &p)
        : pMin(p), pMax(p) {
    }

    FILIANORE_INLINE AABB(const Vector3f &_min, const Vector3f &_max)
        : pMin(_min), pMax(_max) {
    }

    FILIANORE_INLINE static AABB full() {
        float maxF = std::numeric_limits<float>::max();

        return AABB(
            Vector3f(-maxF, -maxF, -maxF),
            Vector3f(maxF, maxF, maxF));
    }

    FILIANORE_INLINE static AABB empty() {
        float maxF = std::numeric_limits<float>::max();

        return AABB(
            Vector3f(maxF, maxF, maxF),
            Vector3f(-maxF, -maxF, -maxF));
    }

    FILIANORE_INLINE void shrink(const AABB &box) {
        pMin = filianore::max(pMin, box.pMin);
        pMax = filianore::min(pMax, box.pMax);
    }

    FILIANORE_INLINE void extend(const Vector3f &p) {
        pMin = filianore::min(pMin, p);
        pMax = filianore::max(pMax, p);
    }

    FILIANORE_INLINE void extend(const AABB &box) {
        pMin = filianore::min(pMin, box.pMin);
        pMax = filianore::max(pMax, box.pMax);
    }

    FILIANORE_INLINE Vector3f diagonal() const {
        return pMax - pMin;
    }

    FILIANORE_INLINE Vector3f center() const {
        return (pMax + pMin) * 0.5f;
    }

    FILIANORE_INLINE float surface_area() const {
        Vector3f d = diagonal();
        return (d.x * d.y + d.x * d.z + d.y * d.z) * 2.f;
    }

    FILIANORE_INLINE float half_area() const {
        Vector3f d = diagonal();
        return (d.x + d.y) * d.z + d.x * d.y;
    }

    FILIANORE_INLINE float volume() const {
        Vector3f d = diagonal();
        return d.x * d.y * d.z;
    }

    FILIANORE_INLINE int largest_axis() const {
        Vector3f d = diagonal();
        int axis = 0;
        if (d.x < d.y)
            axis = 1;
        if (d.y < d.z)
            axis = 2;
        return axis;
    }

    FILIANORE_INLINE int largest_extent() const {
        return diagonal()[largest_axis()];
    }

    FILIANORE_INLINE Vector3f offset(const Vector3f &p) const {
        Vector3f o = p - pMin;
        if (pMax.x > pMin.x)
            o[0] /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o[1] /= pMax.y - pMin.y;
        if (pMax.z > pMin.z)
            o[2] /= pMax.z - pMin.z;
        return Vector3f(o.x, o.y, o.z);
    }

    FILIANORE_INLINE bool intersect(const Ray &ray, float *hitt0, float *hitt1) const {
        Vector3f invRayDir = Vector3f(1.f / ray.dir.x, 1.f / ray.dir.y, 1.f / ray.dir.z);
        Vector3f tbot = vec_mul(invRayDir, (pMin - ray.origin));
        Vector3f ttop = vec_mul(invRayDir, (pMax - ray.origin));

        Vector3f tmin = filianore::min(ttop, tbot);
        Vector3f tmax = filianore::max(ttop, tbot);

        *hitt0 = std::max(std::max(tmin.x, tmin.y), tmin.z);
        *hitt1 = std::min(std::min(tmax.x, tmax.y), tmax.z);

        return !(*hitt0 > *hitt1) && *hitt1 > 0;
    }
};
} // namespace filianore

#endif