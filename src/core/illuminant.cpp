#include "filianore/core/illuminant.h"
#include "filianore/core/scene.h"

namespace filianore
{

    Illuminant::Illuminant(int _types, int _nSamples)
        : types(_types), nSamples(std::max(1, _nSamples))
    {
    }

    Illuminant::~Illuminant() {}

    Color Illuminant::Le(const Ray &ray) const
    {
        return Color(0.f);
    }

    bool VisibilityEvaluator::Unoccluded(const Scene &scene) const
    {
        return !scene.IntersectP(p0.KindleRayTo(p1));
    }

} // namespace filianore