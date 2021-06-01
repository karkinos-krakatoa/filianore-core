#include "filianore/core/microfacet.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    float MicrofacetDistribution::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wh) const
    {
        if (sampleVisibleArea)
            return EvaluateD(wh) * G1(wo) * AbsDot(wo, wh) / AbsCosTheta(wo);
        else
            return EvaluateD(wh) * AbsCosTheta(wh);
    }

}