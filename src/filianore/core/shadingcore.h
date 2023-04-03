#ifndef _SHADING_CORE_H
#define _SHADING_CORE_H

#include "../maths/geometry.h"
#include "../maths/mathutils.h"

namespace filianore {

FILIANORE_STATIC float cos_theta(const Vector3f &w) {
    return w.z;
}

FILIANORE_STATIC float cos_2_theta(const Vector3f &w) {
    return w.z * w.z;
}

FILIANORE_STATIC float abs_cos_theta(const Vector3f &w) {
    return std::abs(w.z);
}

FILIANORE_STATIC float sin_2_theta(const Vector3f &w) {
    return std::max(0.f, 1.f - cos_2_theta(w));
}

FILIANORE_STATIC float sin_theta(const Vector3f &w) {
    return std::sqrt(sin_2_theta(w));
}

FILIANORE_STATIC float tan_theta(const Vector3f &w) {
    return sin_theta(w) / cos_theta(w);
}

FILIANORE_STATIC float tan_2_theta(const Vector3f &w) {
    return sin_2_theta(w) / cos_2_theta(w);
}

FILIANORE_STATIC float cos_phi(const Vector3f &w) {
    float sinTheta = sin_theta(w);
    return (sinTheta == 0.f) ? 1.f : clamp<float>(w.x / sinTheta, -1.f, 1.f);
}

FILIANORE_STATIC float sin_phi(const Vector3f &w) {
    float sinTheta = sin_theta(w);
    return (sinTheta == 0.f) ? 0.f : clamp<float>(w.y / sinTheta, -1.f, 1.f);
}

FILIANORE_STATIC float cos_2_phi(const Vector3f &w) {
    return cos_phi(w) * cos_phi(w);
}

FILIANORE_STATIC float sin_2_phi(const Vector3f &w) {
    return sin_phi(w) * sin_phi(w);
}

FILIANORE_STATIC bool same_hemisphere(const Vector3f &w, const Vector3f &wp) {
    return (w.z * wp.z) > 0;
}

FILIANORE_STATIC Vector2f uniform_sample_disk(const Vector2f &u) {
    float r = std::sqrt(u.x);
    float theta = 2.f * PI * u.y;
    return Vector2f(r * std::cos(theta), r * std::sin(theta));
}

FILIANORE_STATIC Vector2f concentric_sample_disk(const Vector2f &u) {
    // Map uniform random numbers to $[-1,1]^2$
    Vector2f uOffset = u * 2.f - Vector2f(1.f, 1.f);

    // Handle degeneracy at the origin
    if (uOffset.x == 0 && uOffset.y == 0)
        return Vector2f(0.f, 0.f);

    // Apply concentric mapping to point
    float theta, r;
    if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
        r = uOffset.x;
        theta = PI_OVER_4 * (uOffset.y / uOffset.x);
    } else {
        r = uOffset.y;
        theta = PI_OVER_2 - PI_OVER_4 * (uOffset.x / uOffset.y);
    }
    return Vector2f(std::cos(theta), std::sin(theta)) * r;
}

FILIANORE_STATIC Vector3f cosine_hemisphere_sample(const Vector2f &u) {
    Vector2f d = concentric_sample_disk(u);
    float z = std::sqrt(std::max(0.f, 1.f - d.x * d.x - d.y * d.y));
    return Vector3f(d.x, d.y, z);
}

FILIANORE_STATIC float cosine_hemisphere_pdf(float cosTheta) {
    return cosTheta * INV_PI;
}

FILIANORE_STATIC Vector3f uniform_sample_hemisphere(const Vector2f &u) {
    float z = u[0];
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2.f * PI * u[1];
    return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

FILIANORE_STATIC Vector3f uniform_sample_sphere(const Vector2f &u) {
    float z = 1.f - 2.f * u[0];
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2.f * PI * u[1];
    return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

FILIANORE_STATIC float uniform_hemisphere_pdf() {
    return INV_2_PI;
}
} // namespace filianore

#endif