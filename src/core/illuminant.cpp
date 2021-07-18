#include "filianore/core/illuminant.h"
#include "filianore/core/scene.h"

namespace filianore
{

    FILIANORE_INLINE bool IsDeltaIlluminant(int flags)
    {
        return flags & (int)IlluminantType::DeltaPoint || flags & (int)IlluminantType::DeltaDirectional;
    }

    Illuminant::Illuminant(const Transform &_illumToWorld, int _types, int _nSamples, short _decayRate, const PrincipalSpectrum &_shadowColor)
        : illumToWorld(_illumToWorld), types(_types), nSamples(std::max(1, _nSamples)), decayRate(_decayRate), shadowColor(_shadowColor)
    {
    }

    Illuminant::~Illuminant() {}

    PrincipalSpectrum Illuminant::Le(const Ray &ray) const
    {
        return PrincipalSpectrum(0.f);
    }

    float Illuminant::EvaluateDecayRate(const StaticArray<float, 3> &d) const
    {
        float dL = d.Length();
        float strength = dL * dL;

        switch (decayRate)
        {
        case (short)DecayRate::NoDecay:
            strength = 1.f;
        case (short)DecayRate::Linear:
            strength = dL;
        case (short)DecayRate::Quadratic:
            strength = dL * dL;
        case (short)DecayRate::Cubic:
            strength = dL * dL * dL;
        default:
            strength = dL * dL;
        }

        return strength;
    }

    bool VisibilityEvaluator::Unoccluded(const Scene &scene) const
    {
        SurfaceInteraction isect;
        bool hit = scene.Intersect(p0.KindleRayTo(p1.p), &isect);
        if (hit)
        {
            if (isect.primitive->GetAreaIlluminant() != nullptr)
            {
                return true;
            }
            return false;
        }
        return true;
        //return !scene.IntersectP(p0.KindleRayTo(p1.p));
    }

    AreaIlluminant::AreaIlluminant(const Transform &_lightToWorld, short _decayRate, const PrincipalSpectrum &_shadowColor)
        : Illuminant(_lightToWorld, (int)IlluminantType::Area, 1, _decayRate, _shadowColor)
    {
    }

} // namespace filianore