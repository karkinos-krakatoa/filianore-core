#include "filianore/core/interaction.h"
#include "filianore/core/primitive.h"
#include "filianore/core/illuminant.h"

namespace filianore
{

    StaticArray<float, 3> OffsetRayOrigin(const StaticArray<float, 3> &p, const StaticArray<float, 3> &pError,
                                          const StaticArray<float, 3> &n, const StaticArray<float, 3> &w)
    {
        return p + n * Epsilon<float>;
    }

    Color SurfaceInteraction::Le(const StaticArray<float, 3> &w) const
    {
        return Color(0.f);
    }

    void SurfaceInteraction::ComputeScatteringFunctions(const Ray &ray)
    {
        if (primitive)
        {
            primitive->ComputeScatteringFunctions(this);
        }
    }

} // namespace filianore