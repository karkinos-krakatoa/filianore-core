#ifndef _TROWBRIDGE_REITZ_DISTRIBUTION_H
#define _TROWBRIDGE_REITZ_DISTRIBUTION_H

#include "../core/microfacet.h"

namespace filianore
{

    class TrowbridgeReitzDistribution : public MicrofacetDistribution
    {
    public:
        static inline float RoughnessToAlpha(float roughness)
        {
            roughness = std::max(roughness, (float)1e-3);
            float x = std::log(roughness);
            return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
                   0.000640711f * x * x * x * x;
        }

        TrowbridgeReitzDistribution(float alphax, float alphay,
                                    bool samplevis = true)
            : MicrofacetDistribution(samplevis),
              alphax(std::max(float(0.001), alphax)),
              alphay(std::max(float(0.001), alphay))
        {
        }

        float EvaluateD(const StaticArray<float, 3> &wh) const;
        StaticArray<float, 3> SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const;

    private:
        float Lambda(const StaticArray<float, 3> &w) const;

        const float alphax, alphay;
    };
}

#endif