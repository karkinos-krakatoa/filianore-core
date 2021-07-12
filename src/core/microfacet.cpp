#include "filianore/core/microfacet.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    float MicrofacetDistribution::G1(const StaticArray<float, 3> &w) const
    {
        return 1 / (1 + Lambda(w));
    }

    float MicrofacetDistribution::EvaluateG(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return 1 / (1 + Lambda(wo) + Lambda(wi));
    }

    float MicrofacetDistribution::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wh) const
    {
        return EvaluateD(wh) * AbsCosTheta(wh);
    }

}