#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "../shapes/shape.h"
#include <utility>

namespace filianore {
class Primitive {
public:
    Primitive() {}

    virtual AABB world_bound() const = 0;

    virtual bool intersect(const Ray &ray, SurfaceInteraction *isect) const = 0;

    virtual bool intersect_p(const Ray &ray) const = 0;

    virtual Vector3f centroid() const = 0;

    virtual void compute_scattering_functions(SurfaceInteraction *isect) const = 0;

    virtual Material *get_material() const = 0;

    virtual AreaIlluminant *get_area_illuminant() const = 0;
};

class GeometricPrimitive : public Primitive {
public:
    GeometricPrimitive() {}

    GeometricPrimitive(std::shared_ptr<Shape> &_shape, const std::shared_ptr<Material> &_material, const std::shared_ptr<AreaIlluminant> &_areaIllum)
        : shape(_shape), material(_material), areaIllum(_areaIllum) {
    }

    AABB world_bound() const;

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;

    bool intersect_p(const Ray &ray) const;

    Vector3f centroid() const;

    Material *get_material() const;

    AreaIlluminant *get_area_illuminant() const;

    void compute_scattering_functions(SurfaceInteraction *isect) const;

private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaIlluminant> areaIllum;
};

} // namespace filianore

#endif