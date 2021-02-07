#include "filianore/illuminants/spot.h"
#include "filianore/maths/scalar.h"

namespace filianore
{

    SpotIlluminant::SpotIlluminant(const Transform &_lightToWorld, const StaticArray<float, 3> &_dirIllum,
                                   float _coneAngle, float _penumbraAngle, bool angleInRadians, bool usehalfAngles, const RGBSpectrum &_color,
                                   float _intensity, short _decayRate, RGBSpectrum _shadowColor)
        : Illuminant(_lightToWorld, (int)IlluminantType::DeltaPoint, 1, _decayRate, _shadowColor), color(_color), intensity(_intensity)
    {
        float coneAngle = angleInRadians ? _coneAngle : Radians<float>(_coneAngle);
        coneAngle = usehalfAngles ? coneAngle / 2.f : coneAngle;
        cosConeAngle = std::cos(coneAngle);

        float penumbraAngle = angleInRadians ? _penumbraAngle : Radians<float>(_penumbraAngle);
        penumbraAngle = usehalfAngles ? penumbraAngle / 2.f : penumbraAngle;
        cosPenumbraAngle = std::cos(penumbraAngle);

        posIllum = _lightToWorld.PointTransform(StaticArray<float, 3>(0.f));
        dirIllum = _lightToWorld.VectorTransform(_dirIllum);
    }

    RGBSpectrum SpotIlluminant::SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                                         VisibilityEvaluator *visEval) const
    {
        *wi = (posIllum - isect.p).Normalize();
        *pdf = 1.f;

        *visEval = VisibilityEvaluator(isect, Interaction(posIllum, isect.time));

        return (color * intensity * Falloff(StaticArray<float, 3>(-wi->params[0], -wi->params[1], -wi->params[2]))) / EvaluateDecayRate(posIllum - isect.p);
    }

    RGBSpectrum SpotIlluminant::Power() const
    {
        return RGBSpectrum(intensity * 2 * Pi<float> * (1.f - .5f * (cosConeAngle + cosPenumbraAngle)));
    }

    void SpotIlluminant::PrepareIlluminant(const Scene &scene)
    {
    }

    float SpotIlluminant::PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

    float SpotIlluminant::Falloff(const StaticArray<float, 3> &w) const
    {
        StaticArray<float, 3> wNml = w;
        StaticArray<float, 3> dirNml = dirIllum;

        float cosTheta = Dot(wNml.Normalize(), dirNml.Normalize());

        if (cosTheta < cosConeAngle)
            return 0.f;
        if (cosTheta > cosPenumbraAngle)
            return 1.f;

        // float actualFalloff = (cosTheta - cosConeAngle) / (cosPenumbraAngle - cosConeAngle);
        // return actualFalloff * actualFalloff * actualFalloff * actualFalloff * 0.f;
        return 1.f;
    }

} // namespace filianore