#pragma once

#include "../core/elemental.h"
#include <vector>

namespace filianore
{

    class ShapeCreator
    {
    public:
        ShapeCreator() {}

        std::vector<std::shared_ptr<Shape>> CreateQuad(const Transform &_transform);
    };

} // namespace filianore
