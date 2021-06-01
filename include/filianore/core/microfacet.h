#ifndef _MICROFACET_H
#define _MICROFACET_H

#include "../maths/static_array.h"
#include "../color/principalspectrum.h"

namespace filianore
{

    class MicrofacetDistribution
    {
    public:
        virtual ~MicrofacetDistribution() {}

        virtual float EvaluateD(const StaticArray<float, 3> &wh) const = 0;

        virtual float Lambda(const StaticArray<float, 3> &w) const = 0;

        float G1(const StaticArray<float, 3> &w) const
        {
            return 1 / (1 + Lambda(w));
        }

        virtual float EvaluateG(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
        {
            return 1 / (1 + Lambda(wo) + Lambda(wi));
        }

        virtual StaticArray<float, 3> SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const = 0;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wh) const;

    protected:
        MicrofacetDistribution(bool _sampleVisibleArea)
            : sampleVisibleArea(_sampleVisibleArea)
        {
        }

        const bool sampleVisibleArea;
    };

} // namespace filianore

#endif