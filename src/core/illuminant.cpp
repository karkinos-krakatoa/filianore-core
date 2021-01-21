#include "filianore/core/illuminant.h"
#include "filianore/core/scene.h"

namespace filianore
{

    FILIANORE_INLINE bool IsDeltaIlluminant(int flags)
    {
        return flags & (int)IlluminantType::DeltaPoint || flags & (int)IlluminantType::DeltaDirectional;
    }

    Illuminant::Illuminant(int _types, int _nSamples)
        : types(_types), nSamples(std::max(1, _nSamples))
    {
    }

    Illuminant::~Illuminant() {}

    RGBSpectrum Illuminant::Le(const Ray &ray) const
    {
        return RGBSpectrum(0.f);
    }

    bool VisibilityEvaluator::Unoccluded(const Scene &scene) const
    {
        SurfaceInteraction isect;
        bool hit = scene.Intersect(p0.KindleRayTo(p1.p), &isect);
        if (hit)
        {
            return false;
        }
        return true;
    }

} // namespace filianore