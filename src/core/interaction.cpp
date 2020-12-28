#include "filianore/core/interaction.h"
#include "filianore/core/primitive.h"
#include "filianore/core/illuminant.h"

namespace filianore
{

    Color SurfaceInteraction::Le(const StaticArray<float, 3> &w) const
    {
        return Color(0.f);
    }

    void SurfaceInteraction::ComputeScatteringFunctions(const Ray &ray, bool allowMultipleLobes, TransportMode mode)
    {
        primitive->
    }

} // namespace filianore