#ifndef _ESTEVEZ_SHEEN_DISTRIBUTION_H
#define _ESTEVEZ_SHEEN_DISTRIBUTION_H

#include "../../core/microfacet.h"

namespace filianore {

class EstevezSheenDistribution : public MicrofacetDistribution {
public:
    EstevezSheenDistribution(float _roughness);

    float evaluate_distribution(const Vector3f &wh) const;
    Vector3f sample_wh(const Vector3f &wo, const Vector2f &u) const;

private:
    float lambda(const Vector3f &w) const;

    const float roughness;
};
} // namespace filianore

#endif