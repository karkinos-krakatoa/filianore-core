#include "primitive.h"
#include "../maths/geometry.h"
#include "../shading/materials/material.h"
#include "interaction.h"

namespace filianore {

AABB GeometricPrimitive::world_bound() const {
    return shape->world_bound();
}

bool GeometricPrimitive::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    if (!shape->intersect(ray, isect)) {
        return false;
    }
    ray.tMax = isect->t;
    isect->primitive = this;

    return true;
}

bool GeometricPrimitive::intersect_p(const Ray &ray) const {
    return shape->intersect_p(ray);
}

Vector3f GeometricPrimitive::centroid() const {
    return shape->centroid();
}

void GeometricPrimitive::compute_scattering_functions(SurfaceInteraction *isect) const {
    if (material) {
        material->compute_scattering_functions(isect);
    }
}

Material *GeometricPrimitive::get_material() const {
    return material.get();
}

AreaIlluminant *GeometricPrimitive::get_area_illuminant() const {
    return areaIllum.get();
}

} // namespace filianore