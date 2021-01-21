#include "filianore/core/interaction.h"
#include "filianore/core/primitive.h"
#include "filianore/core/illuminant.h"
#include "filianore/maths/vec3_math.h"

namespace filianore
{

    Spectrum<float> SurfaceInteraction::Le(const StaticArray<float, 3> &w) const
    {
        return Spectrum<float>(0.f);
    }

    void SurfaceInteraction::ComputeScatteringFunctions(const Ray &ray)
    {
        if (primitive)
        {
            primitive->ComputeScatteringFunctions(this);
        }
    }

} // namespace filianore