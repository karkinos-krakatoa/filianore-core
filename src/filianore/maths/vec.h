#ifndef _GEOMETRY_VECTOR_H
#define _GEOMETRY_VECTOR_H

#include "../core/elemental.h"
#include "mathutils.h"
#include <iterator>

namespace filianore {

// Vector Declarations
template <typename T>
class Vector2 {
public:
    // Vector2 Public Methods
    Vector2() { x = y = 0; }
    Vector2(T v) : x(v), y(v) {}
    Vector2(T xx, T yy) : x(xx), y(yy) {}
    bool has_nans() const { return is_nan(x) || is_nan(y); }
    explicit Vector2(const Point2<T> &p);
    explicit Vector2(const Point3<T> &p);

    Vector2<T> operator+(const Vector2<T> &v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2<T> &operator+=(const Vector2<T> &v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2<T> operator-(const Vector2<T> &v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2<T> &operator-=(const Vector2<T> &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    bool operator==(const Vector2<T> &v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vector2<T> &v) const { return x != v.x || y != v.y; }

    template <typename U>
    Vector2<T> operator*(U f) const {
        return Vector2<T>(f * x, f * y);
    }

    template <typename U>
    Vector2<T> &operator*=(U f) {
        x *= f;
        y *= f;
        return *this;
    }
    template <typename U>
    Vector2<T> operator/(U f) const {
        T inv = (T)1 / f;
        return Vector2<T>(x * inv, y * inv);
    }

    template <typename U>
    Vector2<T> &operator/=(U f) {
        T inv = (T)1 / f;
        x *= inv;
        y *= inv;
        return *this;
    }
    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
    T operator[](int i) const {
        return i == 0 ? x : y;
    }

    T &operator[](int i) {
        return i == 0 ? x : y;
    }

    T length_squared() const { return x * x + y * y; }
    T length() const { return std::sqrt(length_squared()); }

    // Vector2 Public Data
    T x, y;
};

template <typename T>
class Vector3 {
public:
    // Vector3 Public Methods
    T operator[](int i) const {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }
    T &operator[](int i) {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }
    Vector3() { x = y = z = 0; }
    Vector3(T v) : x(v), y(v), z(v) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    bool has_nans() const { return is_nan(x) || is_nan(y) || is_nan(z); }
    explicit Vector3(const Point3<T> &p);

    Vector3<T> operator+(const Vector3<T> &v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T> &operator+=(const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3<T> operator-(const Vector3<T> &v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T> &operator-=(const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    bool operator==(const Vector3<T> &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector3<T> &v) const {
        return x != v.x || y != v.y || z != v.z;
    }
    template <typename U>
    Vector3<T> operator*(U s) const {
        return Vector3<T>(s * x, s * y, s * z);
    }
    template <typename U>
    Vector3<T> &operator*=(U s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    template <typename U>
    Vector3<T> operator/(U f) const {
        T inv = static_cast<T>(1) / f;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }

    template <typename U>
    Vector3<T> &operator/=(U f) {
        T inv = static_cast<T>(1) / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
    T length_squared() const { return x * x + y * y + z * z; }
    T length() const { return std::sqrt(length_squared()); }

    // Vector3 Public Data
    T x, y, z;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

template <typename T>
FILIANORE_INLINE Vector3<T> min(const Vector3<T> &p1, const Vector3<T> &p2) {
    return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
}

template <typename T>
FILIANORE_INLINE Vector3<T> max(const Vector3<T> &p1, const Vector3<T> &p2) {
    return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
}

template <typename T, typename U>
FILIANORE_INLINE Vector3<T> operator*(U s, const Vector3<T> &v) {
    return v * s;
}

template <typename T>
FILIANORE_INLINE Vector3<T> vec_mul(const Vector3<T> &v1, const Vector3<T> &v2) {
    return Vector3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

template <typename T>
FILIANORE_INLINE Vector3<T> vec_div(const Vector3<T> &v1, const Vector3<T> &v2) {
    return Vector3<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

template <typename T>
FILIANORE_INLINE Vector3<T> abs(const Vector3<T> &v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
FILIANORE_INLINE T dot(const Vector3<T> &v1, const Vector3<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
FILIANORE_INLINE T abs_dot(const Vector3<T> &v1, const Vector3<T> &v2) {
    return std::abs(dot(v1, v2));
}

template <typename T>
FILIANORE_INLINE Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2) {
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
                      (v1x * v2y) - (v1y * v2x));
}

template <typename T>
FILIANORE_INLINE Vector3<T> normalize(const Vector3<T> &v) {
    return v / v.length();
}

template <typename T>
FILIANORE_INLINE T min_component(const Vector3<T> &v) {
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
FILIANORE_INLINE T max_component(const Vector3<T> &v) {
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
FILIANORE_INLINE int max_dimension(const Vector3<T> &v) {
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

template <typename T>
FILIANORE_INLINE Vector3<T> permute(const Vector3<T> &v, int x, int y, int z) {
    return Vector3<T>(v[x], v[y], v[z]);
}

template <typename T>
void coordinate_system(const Vector3<T> &v1, Vector3<T> *v2, Vector3<T> *v3) {
    if (std::abs(v1.x) > std::abs(v1.y)) {
        T invLen = 1.f / std::sqrt(v1.x * v1.x + v1.z * v1.z);
        *v2 = Vector3<T>(v1.z * invLen, 0, -v1.x * invLen);
    } else {
        T invLen = 1.f / std::sqrt(v1.y * v1.y + v1.z * v1.z);
        *v2 = Vector3<T>(0, v1.z * invLen, -v1.y * invLen);
    }
    *v3 = cross(v1, *v2);
}

template <typename T, typename U>
FILIANORE_INLINE Vector2<T> operator*(U f, const Vector2<T> &v) {
    return v * f;
}

template <typename T>
FILIANORE_INLINE T dot(const Vector2<T> &v1, const Vector2<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
FILIANORE_INLINE T abs_dot(const Vector2<T> &v1, const Vector2<T> &v2) {
    return std::abs(dot(v1, v2));
}

template <typename T>
FILIANORE_INLINE Vector2<T> normalize(const Vector2<T> &v) {
    return v / v.length();
}

template <typename T>
Vector2<T> abs(const Vector2<T> &v) {
    return Vector2<T>(std::abs(v.x), std::abs(v.y));
}

template <typename T>
FILIANORE_INLINE Vector3<T> face_forward(const Vector3<T> &v, const Vector3<T> &v2) {
    return (dot(v, v2) < 0.f) ? -v : v;
}

template <typename T>
FILIANORE_INLINE Vector3<T> spherical_direction(T sinTheta, T cosTheta, T phi) {
    return Vector3<T>(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
}

template <typename T>
FILIANORE_INLINE Vector3<T> spherical_direction(T sinTheta, T cosTheta, T phi, const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z) {
    return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y + cosTheta * z;
}

template <typename T>
FILIANORE_INLINE T spherical_theta(const Vector3<T> &v) {
    return std::acos(filianore::clamp<T>(v.z, -1, 1));
}

template <typename T>
FILIANORE_INLINE T spherical_phi(const Vector3<T> &v) {
    T p = std::atan2(v.y, v.x);
    return (p < 0) ? (p + 2 * PI) : p;
}

template <typename T>
FILIANORE_INLINE Vector3<T> reflect(const Vector3<T> &a, const Vector3<T> &b) {
    return b * 2.f * dot(a, b) - a;
}

template <typename T>
FILIANORE_INLINE bool refract(const Vector3<T> &wi, const Vector3<T> &n, const T &eta, Vector3<T> *wt) {
    T cosI = dot(wi, n);
    T sinI2 = std::max(T(0), T(1) - (cosI * cosI));
    T sinT2 = eta * eta * sinI2;

    if (sinT2 >= 1) /* Total Internal Reflection */
    {
        return false;
    }

    T cosT = std::sqrt(1 - sinT2);
    *wt = wi * -eta + n * (eta * cosI - cosT);
    return true;
}

} // namespace filianore

#endif