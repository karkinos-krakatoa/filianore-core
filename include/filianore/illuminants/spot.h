#ifndef _ILLUMINANT_SPOT_H
#define _ILLUMINANT_SPOT_H

#include "../core/illuminant.h"

namespace filianore
{

    class SpotIlluminant : public Illuminant
    {
    public:
        SpotIlluminant(const Transform &_lightToWorld, const StaticArray<float, 3> &_dirIllum,
                       float _coneAngle, float _penumbraAngle, bool angleInRadians, bool usehalfAngles, const PrincipalSpectrum &_color,
                       float _intensity, short _decayRate, const PrincipalSpectrum &_shadowColor);

        PrincipalSpectrum SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                                   VisibilityEvaluator *visEval) const;

        PrincipalSpectrum Power() const;

        void PrepareIlluminant(const Scene &scene);

        float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const;

    private:
        float Falloff(const StaticArray<float, 3> &w) const;

        StaticArray<float, 3> posIllum, dirIllum;
        const float intensity;
        const PrincipalSpectrum color;
        float cosConeAngle, cosPenumbraAngle;
    };

} // namespace filianore

#endif