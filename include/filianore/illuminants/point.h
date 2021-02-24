#ifndef _ILLUMINANT_POINT_H
#define _ILLUMINANT_POINT_H

#include "../core/illuminant.h"

namespace filianore
{

    class PointIlluminant : public Illuminant
    {
    public:
        PointIlluminant(const Transform &_lightToWorld, const PrincipalSpectrum &_color, float _intensity, short _decayRate, const PrincipalSpectrum &_shadowColor);

        PrincipalSpectrum SampleLi(const Interaction &isect, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf,
                                   VisibilityEvaluator *visEval) const;

        PrincipalSpectrum Power() const;

        void PrepareIlluminant(const Scene &scene);

        float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const;

    private:
        StaticArray<float, 3> posIllum;
        const float intensity;
        const PrincipalSpectrum color;
    };

} // namespace filianore

#endif