#include "filianore/creators/shapecreator.h"
#include "filianore/maths/transform.h"
#include "filianore/shapes/triangle.h"

namespace filianore {

std::vector<std::shared_ptr<Shape>> ShapeCreator::create_quad(const Transform &_transform) {
    // First Triangle
    Vector3f t1_v1(1.f, -1.f, 0.f);
    Vector3f t1_v2(-1.f, -1.f, 0.f);
    Vector3f t1_v3(-1.f, 1.f, 0.f);

    // Second Triangle
    Vector3f t2_v1(1.f, -1.f, 0.f);
    Vector3f t2_v2(-1.f, 1.f, 0.f);
    Vector3f t2_v3(1.f, 1.f, 0.f);

    std::vector<Vector3f> vertices = {t1_v1, t1_v2, t1_v3, t2_v1, t2_v2, t2_v3};

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = _transform.point_transform(vertices[i]);
    }

    TriangleEntity t1_e1(vertices[0], Vector3f());
    TriangleEntity t1_e2(vertices[1], Vector3f());
    TriangleEntity t1_e3(vertices[2], Vector3f());

    TriangleEntity t2_e1(vertices[3], Vector3f());
    TriangleEntity t2_e2(vertices[4], Vector3f());
    TriangleEntity t2_e3(vertices[5], Vector3f());

    std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(t1_e1, t1_e2, t1_e3, false);
    std::shared_ptr<Shape> t2 = std::make_shared<Triangle>(t2_e1, t2_e2, t2_e3, false);

    std::vector<std::shared_ptr<Shape>> quad = {t1, t2};

    return quad;
}

} // namespace filianore
