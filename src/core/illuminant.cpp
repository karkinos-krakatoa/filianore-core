#include "filianore/core/illuminant.h"
#include "filianore/core/scene.h"

namespace filianore
{

    FILIANORE_INLINE bool IsDeltaIlluminant(int flags)
    {
        return flags & (int)IlluminantType::DeltaPoint || flags & (int)IlluminantType::DeltaDirectional;
    }

    Illuminant::Illuminant(const Transform &_illumToWorld, int _types, int _nSamples, short _decayRate, RGBSpectrum _shadowColor)
        : illumToWorld(_illumToWorld), types(_types), nSamples(std::max(1, _nSamples)), decayRate(_decayRate), shadowColor(_shadowColor)
    {
    }

    Illuminant::~Illuminant() {}

    RGBSpectrum Illuminant::Le(const Ray &ray) const
    {
        return RGBSpectrum(0.f);
    }

    float Illuminant::EvaluateDecayRate(const StaticArray<float, 3> &d) const
    {
        float dL = d.Length();

        switch (decayRate)
        {
        case (short)DecayRate::NoDecay:
            return 1.f;
        case (short)DecayRate::Linear:
            return dL;
        case (short)DecayRate::Quadratic:
            return dL * dL;
        case (short)DecayRate::Cubic:
            return dL * dL * dL;
        default:
            return dL * dL;
        }
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