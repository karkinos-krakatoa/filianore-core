#include "../microfacets/beckmann.h"
#include "../../core/shadingcore.h"
#include "../../maths/vec.h"

namespace filianore {

BeckmannDistribution::BeckmannDistribution(float _alphax, float _alphay)
    : alphax(_alphax),
      alphay(_alphay) {
}

float BeckmannDistribution::evaluate_distribution(const Vector3f &wh) const {
    float tan2Theta = tan_2_theta(wh);
    if (std::isinf(tan2Theta))
        return 0.f;
    float cos4Theta = cos_2_theta(wh) * cos_2_theta(wh);
    return std::exp(-tan2Theta * (cos_2_phi(wh) / (alphax * alphax) +
                                  sin_2_phi(wh) / (alphay * alphay))) /
           (PI * alphax * alphay * cos4Theta);
}

float BeckmannDistribution::lambda(const Vector3f &w) const {
    float absTanTheta = std::abs(tan_theta(w));
    if (std::isinf(absTanTheta))
        return 0.f;
    float alpha = std::sqrt(cos_2_phi(w) * alphax * alphax +
                            sin_2_phi(w) * alphay * alphay);

    float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}

Vector3f BeckmannDistribution::sample_wh(const Vector3f &wo, const Vector2f &u) const {
    float tan2Theta, phi;
    if (alphax == alphay) {
        float logSample = std::log(1 - u.x);
        tan2Theta = -alphax * alphax * logSample;
        phi = u.y * 2.f * PI;
    } else {
        float logSample = std::log(1 - u.x);
        phi = std::atan(alphay / alphax * std::tan(2 * PI * u.y + 0.5f * PI));
        if (u.y > 0.5f)
            phi += PI;
        float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
        float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
        tan2Theta = -logSample / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
    }

    float cosTheta = 1 / std::sqrt(1 + tan2Theta);
    float sinTheta = std::sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
    Vector3f wh = spherical_direction<float>(sinTheta, cosTheta, phi);
    if (!same_hemisphere(wo, wh))
        wh = -wh;
    return wh;
}
} // namespace filianore