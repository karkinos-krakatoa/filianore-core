#ifndef _GGX_DISTRIBUTION_H
#define _GGX_DISTRIBUTION_H

#include "../../core/microfacet.h"

namespace filianore
{

    class GGXDistribution : public MicrofacetDistribution
    {
    public:
        GGXDistribution(float _alphax, float _alphay);

        static inline float RoughnessToAlpha(float roughness);

        float EvaluateD(const StaticArray<float, 3> &wh) const;
        StaticArray<float, 3> SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const;

    private:
        float Lambda(const StaticArray<float, 3> &w) const;

        const float alphax, alphay;
    };

    inline float GGXDistribution::RoughnessToAlpha(float roughness)
    {
        roughness = std::max(roughness, (float)1e-3);
        float x = std::log(roughness);
        return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
               0.000640711f * x * x * x * x;
    }
}

#endif