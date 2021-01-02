#ifndef _ILLUMINANT_POINT_H
#define _ILLUMINANT_POINT_H

#include "../core/illuminant.h"

namespace filianore
{

    class PointIlluminant : public Illuminant
    {
    public:
        PointIlluminant(const StaticArray<float, 3> &_illumPosition, const Color &_I, float _Iw)
            : Illuminant((int)IlluminantType::DeltaPoint), illumPosition(_illumPosition), I(_I), Iw(_Iw)
        {
        }

        Color SampleLi(const Interaction &ref, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf, VisibilityEvaluator *vis) const;
        Color Power() const;
        Color Le(const Ray &ray) const;
        float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const;

    private:
        StaticArray<float, 3> illumPosition;
        Color I;
        float Iw;
    };

} // namespace filianore

#endif