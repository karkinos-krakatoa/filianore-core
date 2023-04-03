#ifndef _TRIANGLE_SHAPE_H
#define _TRIANGLE_SHAPE_H

#include "../maths/geometry.h"
#include "shape.h"

namespace filianore {

struct TriangleEntity {
    Vector3f vertex;
    Vector3f normal;
    Vector3f tangent;
    bool normalFound, tangentFound;
    Vector2f uv;

    TriangleEntity()
        : vertex(Vector3f()), normal(Vector3f()), tangent(Vector3f()), normalFound(false), uv(Vector2f()) {
    }

    TriangleEntity(const Vector3f &_v, const Vector3f &_n = Vector3f(), bool _normalFound = false,
                   const Vector3f &_t = Vector3f(), bool _tangentFound = false,
                   const Vector2f &_uv = Vector2f())
        : vertex(_v), normal(_n), normalFound(_normalFound), tangent(_t), tangentFound(_tangentFound), uv(_uv) {
    }
};

class Triangle : public Shape {
public:
    Triangle() {}

    Triangle(const TriangleEntity &_v1, const TriangleEntity &_v2, const TriangleEntity &_v3, bool _reverseOrientation = false)
        : Shape(_reverseOrientation), v1(_v1), v2(_v2), v3(_v3) {
        allNormalsInMesh = (v1.normalFound && v2.normalFound && v3.normalFound) ? true : false;
        allTangentsInMesh = (v1.tangentFound && v2.tangentFound && v3.tangentFound) ? true : false;
    }

    AABB world_bound() const;

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;

    Vector3f centroid() const;

    float area() const;

    using Shape::sample;
    Interaction sample(const Vector2f &u, float *pdf) const;

private:
    TriangleEntity v1, v2, v3;
    bool allNormalsInMesh;
    bool allTangentsInMesh;
};

} // namespace filianore

#endif