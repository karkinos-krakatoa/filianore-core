#include "estevez.h"
#include "../../core/shadingcore.h"
#include "../../maths/geometry.h"

namespace filianore {

EstevezSheenDistribution::EstevezSheenDistribution(float _roughness)
    : roughness(_roughness) {
}

float EstevezSheenDistribution::evaluate_distribution(const Vector3f &wh) const {
    float invRoughness = 1.f / roughness;
    return (2.f + invRoughness) * std::pow(sin_theta(wh), invRoughness) / (2.f * PI);
}

float EstevezSheenDistribution::lambda(const Vector3f &w) const {
    auto paramLerp = [](float r, float p0, float p1) { return ((1.f - r) * (1.f - r)) * p0 + (1.f - ((1.f - r) * (1.f - r))) * p1; };

    float a = paramLerp(roughness, 25.3245f, 21.5473f);
    float b = paramLerp(roughness, 3.32435f, 3.82987f);
    float c = paramLerp(roughness, 0.16801f, 0.19823f);
    float d = paramLerp(roughness, -1.27393f, -1.97760f);
    float e = paramLerp(roughness, -4.85967f, -4.32054f);

    auto L = [a, b, c, d, e](float v) { return a / (1.f + b * std::pow(v, c)) + (d * v) + e; };

    float absCosTheta = abs_cos_theta(w);
    if (absCosTheta < 0.5f) {
        return std::exp(L(absCosTheta));
    }

    float expValue = 2.f * L(0.5f) - L(1.f - absCosTheta);
    return std::exp(expValue);
}

Vector3f EstevezSheenDistribution::sample_wh(const Vector3f &wo, const Vector2f &u) const {
    Vector3f wi = cosine_hemisphere_sample(u);
    if (wo.z < 0) {
        wi[2] *= -1.f;
    }
    return wi;
}
} // namespace filianore