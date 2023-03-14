#include "filianore/shading/microfacets/ggx.h"
#include "filianore/core/shadingcore.h"
#include "filianore/maths/vec.h"

namespace filianore {

GGXDistribution::GGXDistribution(float _alphax, float _alphay)
    : alphax(std::max(0.001f, _alphax)),
      alphay(std::max(0.001f, _alphay)) {
}

float GGXDistribution::evaluate_distribution(const Vector3f &wh) const {
    float tan2Theta = tan_2_theta(wh);
    if (std::isinf(tan2Theta))
        return 0.;

    const float cos4Theta = cos_2_theta(wh) * cos_2_theta(wh);
    float e = (cos_2_phi(wh) / (alphax * alphax) + sin_2_phi(wh) / (alphay * alphay)) * tan2Theta;
    return 1.f / (PI * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

float GGXDistribution::lambda(const Vector3f &w) const {
    float absTanTheta = std::abs(tan_theta(w));
    if (std::isinf(absTanTheta))
        return 0.;

    float alpha = std::sqrt(cos_2_phi(w) * alphax * alphax + sin_2_phi(w) * alphay * alphay);
    float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}

Vector3f GGXDistribution::sample_wh(const Vector3f &wo, const Vector2f &u) const {
    // Transform View Direction in Ellipsoid config to Hemisphere config
    Vector3f vh = normalize(Vector3f(wo.x * alphax, wo.y * alphay, wo.z));
    if (vh.z < 0) {
        vh = -vh;
    }

    // Orthonormal Basis
    float lensq = vh.x * vh.x + vh.y * vh.y;
    Vector3f T1 = lensq > 0 ? Vector3f(-vh.y, vh.x, 0.f) / std::sqrt(lensq) : Vector3f(1.f, 0.f, 0.f);
    Vector3f T2 = cross(vh, T1);

    // Parameterization of the Projected area
    float r = std::sqrt(u.x);
    float phi = 2.f * PI * u.y;
    float t1 = r * std::cos(phi);
    float t2 = r * std::sin(phi);
    float s = (1.f + vh.z) * 0.5;
    t2 = (1.f - s) * std::sqrt(1 - (t1 * t1)) + (s * t2);

    // Reprojection onto Hemisphere
    Vector3f Nh = T1 * t1 + T2 * t2 + vh * (std::sqrt(std::max(0.f, 1.f - t1 * t1 - t2 * t2)));

    // Transform back to Ellipsoid config
    return normalize(Vector3f(alphax * Nh.x, alphay * Nh.y, std::max<float>(1e-6f, Nh.z)));
}
} // namespace filianore