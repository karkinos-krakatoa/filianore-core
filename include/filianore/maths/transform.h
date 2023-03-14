#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "matrix.h"
#include "vec.h"
#include <assert.h>

namespace filianore {

class Transform {
public:
    Matrix<float, 4, 4> mat, matInv;

    Transform() {}

    Transform(const float _mat[4][4]) {
        mat = Matrix<float, 4, 4>(_mat[0][0], _mat[0][1], _mat[0][2], _mat[0][3],
                                  _mat[1][0], _mat[1][1], _mat[1][2], _mat[1][3],
                                  _mat[2][0], _mat[2][1], _mat[2][2], _mat[2][3],
                                  _mat[3][0], _mat[3][1], _mat[3][2], _mat[3][3]);
        matInv = inverse<float>(mat);
    }

    Transform(const Matrix<float, 4, 4> &_mat)
        : mat(_mat), matInv(inverse<float>(_mat)) {}

    Transform(const Matrix<float, 4, 4> &_mat, const Matrix<float, 4, 4> &_matInv)
        : mat(_mat), matInv(_matInv) {}

    Transform operator*(Transform t2) {
        Matrix<float, 4, 4> m1 = mat * t2.mat;
        Matrix<float, 4, 4> m2 = t2.matInv * matInv;
        return Transform(m1, m2);
    }

    bool operator==(const Transform &t) const {
        return t.mat == mat && t.matInv == matInv;
    }

    bool operator!=(const Transform &t) const {
        return t.mat != mat || t.matInv != matInv;
    }

    Vector3f point_transform(const Vector3f &pt) const {
        float xp = mat.data[0][0] * pt.x + mat.data[0][1] * pt.y + mat.data[0][2] * pt.z + mat.data[0][3];
        float yp = mat.data[1][0] * pt.x + mat.data[1][1] * pt.y + mat.data[1][2] * pt.z + mat.data[1][3];
        float zp = mat.data[2][0] * pt.x + mat.data[2][1] * pt.y + mat.data[2][2] * pt.z + mat.data[2][3];
        float wp = mat.data[3][0] * pt.x + mat.data[3][1] * pt.y + mat.data[3][2] * pt.z + mat.data[3][3];

        if (wp == 1.0) {
            return Vector3f(xp, yp, zp);
        } else {
            assert(wp != 0);
            return Vector3f(xp, yp, zp) / wp;
        }
    }

    Vector3f vector_transform(const Vector3f &v) const {
        return Vector3f(mat.data[0][0] * v.x + mat.data[0][1] * v.y + mat.data[0][2] * v.z,
                        mat.data[1][0] * v.x + mat.data[1][1] * v.y + mat.data[1][2] * v.z,
                        mat.data[2][0] * v.x + mat.data[2][1] * v.y + mat.data[2][2] * v.z);
    }

    friend Transform inverse(const Transform &t) {
        return Transform(t.matInv, t.mat);
    }

    bool swaps_handedness() const {
        float det = mat.data[0][0] * (mat.data[1][1] * mat.data[2][2] - mat.data[1][2] * mat.data[2][1]) -
                    mat.data[0][1] * (mat.data[1][0] * mat.data[2][2] - mat.data[1][2] * mat.data[2][0]) +
                    mat.data[0][2] * (mat.data[1][0] * mat.data[2][1] - mat.data[1][1] * mat.data[2][0]);
        return det < 0;
    }
};

static Transform translate(float x, float y, float z) {
    Matrix<float, 4, 4> m(1.f, 0.f, 0.f, x,
                          0.f, 1.f, 0.f, y,
                          0.f, 0.f, 1.f, z,
                          0.f, 0.f, 0.f, 1.f);

    Matrix<float, 4, 4> mInv(1.f, 0.f, 0.f, -x,
                             0.f, 1.f, 0.f, -y,
                             0.f, 0.f, 1.f, -z,
                             0.f, 0.f, 0.f, 1.f);

    return Transform(m, mInv);
}

static Transform scale(float x, float y, float z) {
    assert(x != 0 && y != 0 && z != 0);

    Matrix<float, 4, 4> m(x, 0.f, 0.f, 0.f,
                          0.f, y, 0.f, 0.f,
                          0.f, 0, z, 0.f,
                          0.f, 0.f, 0.f, 1.f);

    Matrix<float, 4, 4> mInv(1.f / x, 0.f, 0.f, 0.f,
                             0.f, 1.f / y, 0, 0,
                             0.f, 0, 1.f / z, 0,
                             0.f, 0.f, 0.f, 1.f);

    return Transform(m, mInv);
}

static Transform rotate_x(float angle, bool inRadians = false) {
    float sin_t = std::sin(angle);
    float cos_t = std::cos(angle);

    if (!inRadians) {
        sin_t = std::sin(filianore::radians<float>(angle));
        cos_t = std::cos(filianore::radians<float>(angle));
    }

    Matrix<float, 4, 4> m(1.f, 0.f, 0.f, 0.f,
                          0.f, cos_t, -sin_t, 0.f,
                          0.f, sin_t, cos_t, 0.f,
                          0.f, 0.f, 0.f, 1.f);

    return Transform(m, transpose(m));
}

static Transform rotate_y(float angle, bool inRadians = false) {
    float sin_t = std::sin(angle);
    float cos_t = std::cos(angle);

    if (!inRadians) {
        sin_t = std::sin(filianore::radians<float>(angle));
        cos_t = std::cos(filianore::radians<float>(angle));
    }

    Matrix<float, 4, 4> m(cos_t, 0.f, sin_t, 0.f,
                          0.f, 1.f, 0.f, 0.f,
                          -sin_t, 0.f, cos_t, 0.f,
                          0.f, 0.f, 0.f, 1.f);

    return Transform(m, transpose(m));
}

static Transform rotate_z(float angle, bool inRadians = false) {
    float sin_t = std::sin(angle);
    float cos_t = std::cos(angle);

    if (!inRadians) {
        sin_t = std::sin(filianore::radians<float>(angle));
        cos_t = std::cos(filianore::radians<float>(angle));
    }

    Matrix<float, 4, 4> m(cos_t, -sin_t, 0.f, 0.f,
                          sin_t, cos_t, 0.f, 0.f,
                          0.f, 0.f, 1.f, 0.f,
                          0.f, 0.f, 0.f, 1.f);

    return Transform(m, transpose(m));
}

} // namespace filianore

#endif