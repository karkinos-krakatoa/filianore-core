#include "filianore/creators/shapecreator.h"
#include "filianore/shapes/triangle.h"
#include "filianore/maths/transform.h"

namespace filianore
{

    std::vector<std::shared_ptr<Shape>> ShapeCreator::CreateQuad(const Transform &_transform)
    {
        // First Triangle
        StaticArray<float, 3> t1_v1(1.f, 1.f, 1.f);
        StaticArray<float, 3> t1_v2(1.f, -1.f, 1.f);
        StaticArray<float, 3> t1_v3(-1.f, 1.f, 1.f);

        // Second Triangle
        StaticArray<float, 3> t2_v1(-1.f, 1.f, 1.f);
        StaticArray<float, 3> t2_v2(1.f, -1.f, 1.f);
        StaticArray<float, 3> t2_v3(-1.f, -1.f, 1.f);

        std::vector<StaticArray<float, 3>> vertices = {t1_v1, t1_v2, t1_v3, t2_v1, t2_v2, t2_v3};

        // for (size_t i = 0; i < vertices.size(); ++i)
        // {
        //     vertices[i] = _transform.PointTransform(vertices[i]);
        // }

        TriangleEntity t1_e1(vertices[0], StaticArray<float, 3>());
        TriangleEntity t1_e2(vertices[1], StaticArray<float, 3>());
        TriangleEntity t1_e3(vertices[2], StaticArray<float, 3>());

        TriangleEntity t2_e1(vertices[3], StaticArray<float, 3>());
        TriangleEntity t2_e2(vertices[4], StaticArray<float, 3>());
        TriangleEntity t2_e3(vertices[5], StaticArray<float, 3>());

        std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(t1_e1, t1_e2, t1_e3, false);
        std::shared_ptr<Shape> t2 = std::make_shared<Triangle>(t2_e1, t2_e2, t2_e3, false);

        std::vector<std::shared_ptr<Shape>> quad = {t1, t2};

        return quad;
    }

} // namespace filianore
