#include "filianore/core/microfacet.h"
#include "filianore/core/shadingcore.h"

namespace filianore {

float MicrofacetDistribution::G1(const Vector3f &w) const {
    return 1 / (1 + lambda(w));
}

float MicrofacetDistribution::evaluate_geometric(const Vector3f &wo, const Vector3f &wi) const {
    return 1 / (1 + lambda(wo) + lambda(wi));
}

float MicrofacetDistribution::pdf(const Vector3f &wo, const Vector3f &wh) const {
    return evaluate_distribution(wh) * abs_cos_theta(wh);
}

} // namespace filianore