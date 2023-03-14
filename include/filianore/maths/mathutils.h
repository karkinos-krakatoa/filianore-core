#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include "../core/elemental.h"
#include <cmath>
#include <numeric>
#include <random>
#include <stdint.h>

namespace filianore {

// Constants
#ifdef _MSC_VER
#define MAX_float std::numeric_limits<float>::max()
#define INFINITI (std::numeric_limits<float>::infinity())
#else
static constexpr float INFINITI = std::numeric_limits<float>::infinity();
static constexpr float MAX_FLOAT = std::numeric_limits<float>::max();
#endif
static constexpr float SHADOW_EPSILON = 0.0001f;
static constexpr float PI = 3.14159265358979323846f;
static constexpr float INV_PI = 0.31830988618379067154f;
static constexpr float INV_2_PI = 0.15915494309189533577f;
static constexpr float INV_4_PI = 0.07957747154594766788f;
static constexpr float PI_OVER_2 = 1.57079632679489661923f;
static constexpr float PI_OVER_4 = 0.78539816339744830961f;
static constexpr float SQRT_2 = 1.41421356237309504880f;

// Functions
template <typename T>
FILIANORE_INLINE T lerp(T t, T v1, T v2) {
    return (static_cast<T>(1) - t) * v1 + t * v2;
}

template <typename T>
FILIANORE_INLINE T max_scalar() {
    return std::numeric_limits<T>::max();
}

template <typename T>
FILIANORE_INLINE T machine_epsilon() {
    return std::numeric_limits<T>::epsilon() * static_cast<T>(0.5);
}

template <typename T>
FILIANORE_INLINE T gamma(int n) {
    return (n * machine_epsilon<T>()) / (1.f - n * machine_epsilon<T>());
}

template <typename T>
FILIANORE_INLINE T clamp(T val, T low, T high) {
    if (val < low)
        return low;
    return val > high ? high : val;
}

template <typename T>
FILIANORE_INLINE T random_value() {
    static std::uniform_real_distribution<T> distribution(static_cast<T>(0), static_cast<T>(1));
    static std::mt19937 generator;
    return distribution(generator);
}

template <typename T>
FILIANORE_INLINE T random_value(T min, T max) {
    return min + (max - min) * random_value<T>();
}

template <typename T>
FILIANORE_INLINE T radians(T degrees) {
    return (PI / static_cast<T>(180)) * degrees;
}

template <typename T>
FILIANORE_INLINE T degrees(T radians) {
    return (static_cast<T>(180) / PI) * radians;
}

template <typename P>
int find_interval(int size, const P &predicate) {
    int first = 0, len = size;

    while (len > 0) {
        int half = len >> 1;
        int middle = first + half;
        if (predicate(middle)) {
            first = middle + 1;
            len -= half + 1;
        } else {
            len = half;
        }
    }

    return clamp<int>(first - 1, 0, size - 2);
}

template <typename T>
T next_float_up(float v) {
    // Handle infinity and negative zero for _NextFloatUp()_
    if (std::isinf(v) && v > 0.)
        return v;
    if (v == -0.f)
        v = 0.f;

    // Advance _v_ to next higher float
    uint32_t ui = float_to_bits(v);
    if (v >= 0)
        ++ui;
    else
        --ui;
    return bits_to_float(ui);
}

template <typename T>
inline T next_float_down(float v) {
    // Handle infinity and positive zero for _NextFloatDown()_
    if (std::isinf(v) && v < 0.)
        return v;
    if (v == 0.f)
        v = -0.f;
    uint32_t ui = float_to_bits(v);
    if (v > 0)
        --ui;
    else
        ++ui;
    return bits_to_float(ui);
}

} // namespace filianore

#endif