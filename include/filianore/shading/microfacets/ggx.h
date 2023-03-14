#ifndef _GGX_DISTRIBUTION_H
#define _GGX_DISTRIBUTION_H

#include "../../core/microfacet.h"

namespace filianore {

class GGXDistribution : public MicrofacetDistribution {
public:
    GGXDistribution(float _alphax, float _alphay);

    static inline float roughness_to_alpha(float roughness);

    float evaluate_distribution(const Vector3f &wh) const;
    Vector3f sample_wh(const Vector3f &wo, const Vector2f &u) const;

private:
    float lambda(const Vector3f &w) const;

    const float alphax, alphay;
};

inline float GGXDistribution::roughness_to_alpha(float roughness) {
    roughness = std::max(roughness, (float)1e-3);
    float x = std::log(roughness);
    return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
           0.000640711f * x * x * x * x;
}
} // namespace filianore

#endif