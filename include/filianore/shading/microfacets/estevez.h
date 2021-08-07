#ifndef _ESTEVEZ_SHEEN_DISTRIBUTION_H
#define _ESTEVEZ_SHEEN_DISTRIBUTION_H

#include "../../core/microfacet.h"

namespace filianore
{

    class EstevezSheenDistribution : public MicrofacetDistribution
    {
    public:
        EstevezSheenDistribution(float _roughness);

        float EvaluateD(const StaticArray<float, 3> &wh) const;
        StaticArray<float, 3> SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const;

    private:
        float Lambda(const StaticArray<float, 3> &w) const;

        const float roughness;
    };
}

#endif