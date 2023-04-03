#ifndef _SHAPE_H
#define _SHAPE_H

#include "../core/elemental.h"
#include "../maths/affine.h"

namespace filianore {

class Shape {
public:
    const bool reverseOrientation;

    Shape() : reverseOrientation(false) {}

    Shape(bool _reverseOrientation) : reverseOrientation(_reverseOrientation) {}

    virtual AABB world_bound() const = 0;

    virtual bool intersect(const Ray &ray, SurfaceInteraction *t) const = 0;

    virtual bool intersect_p(const Ray &ray) const;

    virtual Vector3f centroid() const = 0;

    virtual float area() const = 0;

    virtual float pdf(const Interaction &isect) const;
    virtual float pdf(const Interaction &isect, const Vector3f &wi) const;

    virtual Interaction sample(const Vector2f &u, float *pdf) const = 0;
    virtual Interaction sample(const Interaction &isect, const Vector2f &u, float *pdf) const;
};

} // namespace filianore

#endif