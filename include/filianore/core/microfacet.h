#ifndef _MICROFACET_H
#define _MICROFACET_H

#include "../color/principalspectrum.h"
#include "../maths/vec.h"

namespace filianore {

class MicrofacetDistribution {
public:
    MicrofacetDistribution() {}

    virtual ~MicrofacetDistribution() {}

    virtual float evaluate_distribution(const Vector3f &wh) const = 0;

    virtual float lambda(const Vector3f &w) const = 0;

    float G1(const Vector3f &w) const;

    virtual float evaluate_geometric(const Vector3f &wo, const Vector3f &wi) const;

    virtual Vector3f sample_wh(const Vector3f &wo, const Vector2f &u) const = 0;

    float pdf(const Vector3f &wo, const Vector3f &wh) const;
};

} // namespace filianore

#endif