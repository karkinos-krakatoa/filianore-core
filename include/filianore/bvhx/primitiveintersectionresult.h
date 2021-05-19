#ifndef _PRIMITIVE_INTERSECTION_RESULT_H
#define _PRIMITIVE_INTERSECTION_RESULT_H

#include "../core/interaction.h"

namespace filianore
{
    struct PrimitiveIntersectionResult
    {
        size_t primitiveIndex;
        SurfaceInteraction isect;

        float Distance() const { return isect.t; }
    };
}

#endif