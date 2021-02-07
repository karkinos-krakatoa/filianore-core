#ifndef _ILLUMINANT_H
#define _ILLUMINANT_H

#include "elemental.h"
#include "interaction.h"

namespace filianore
{

    enum class IlluminantType : int
    {
        DeltaPoint = 1,
        DeltaDirectional = 2,
        Area = 4,
        Infinite = 8
    };

    enum class DecayRate : int
    {
        NoDecay = 0,
        Linear = 1,
        Quadratic = 2,
        Cubic = 3
    };

    FILIANORE_INLINE bool
    IsDeltaIlluminant(int flags);

    class VisibilityEvaluator
    {
    public:
        VisibilityEvaluator() {}

        VisibilityEvaluator(const Interaction &_p0, const Interaction &_p1)
            : p0(_p0), p1(_p1) {}

        const Interaction &P0() const { return p0; }
        const Interaction &P1() const { return p1; }
        bool Unoccluded(const Scene &scene) const;

    private:
        Interaction p0, p1;
    };

    class Illuminant
    {
    public:
        virtual ~Illuminant();
        Illuminant(const Transform &_illumToWorld, int _types, int _nSamples = 1, short _decayRate = 2, RGBSpectrum _shadowColor = RGBSpectrum(0.f));

        virtual RGBSpectrum SampleLi(const Interaction &ref, const StaticArray<float, 2> &u, StaticArray<float, 3> *wi, float *pdf, VisibilityEvaluator *vis) const = 0;
        virtual RGBSpectrum Power() const = 0;
        virtual void PrepareIlluminant(const Scene &scene) {}
        virtual RGBSpectrum Le(const Ray &ray) const;
        virtual float PdfLi(const Interaction &ref, const StaticArray<float, 3> &wi) const = 0;

        float EvaluateDecayRate(const StaticArray<float, 3> &d) const;

        const int types;
        const int nSamples;
        const short decayRate;
        const RGBSpectrum shadowColor;
        const Transform illumToWorld;
    };

} // namespace filianore

#endif