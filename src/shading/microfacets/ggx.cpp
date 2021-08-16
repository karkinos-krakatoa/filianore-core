#include "filianore/shading/microfacets/ggx.h"
#include "filianore/core/shadingcore.h"
#include "filianore/maths/vec3_math.h"

namespace filianore
{

    GGXDistribution::GGXDistribution(float _alphax, float _alphay)
        : alphax(std::max(0.001f, _alphax)),
          alphay(std::max(0.001f, _alphay))
    {
    }

    float GGXDistribution::EvaluateD(const StaticArray<float, 3> &wh) const
    {
        float tan2Theta = Tan2Theta(wh);
        if (std::isinf(tan2Theta))
            return 0.;

        const float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
        float e = (Cos2Phi(wh) / (alphax * alphax) + Sin2Phi(wh) / (alphay * alphay)) * tan2Theta;
        return 1.f / (Pi<float> * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
    }

    float GGXDistribution::Lambda(const StaticArray<float, 3> &w) const
    {
        float absTanTheta = std::abs(TanTheta(w));
        if (std::isinf(absTanTheta))
            return 0.;

        float alpha = std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
        float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
        return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
    }

    StaticArray<float, 3> GGXDistribution::SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const
    {
        // Transform View Direction in Ellipsoid config to Hemisphere config
        StaticArray<float, 3> vh = (StaticArray<float, 3>(wo.x() * alphax, wo.y() * alphay, wo.z())).Normalize();
        if (vh.z() < 0)
        {
            vh = vh.Neg();
        }

        // Orthonormal Basis
        float lensq = vh.x() * vh.x() + vh.y() * vh.y();
        StaticArray<float, 3> T1 = lensq > 0 ? StaticArray<float, 3>(-vh.y(), vh.x(), 0.f) / std::sqrt(lensq) : StaticArray<float, 3>(1.f, 0.f, 0.f);
        StaticArray<float, 3> T2 = Cross(vh, T1);

        // Parameterization of the Projected Area
        float r = std::sqrt(u.x());
        float phi = 2.f * Pi<float> * u.y();
        float t1 = r * std::cos(phi);
        float t2 = r * std::sin(phi);
        float s = (1.f + vh.z()) * 0.5;
        t2 = (1.f - s) * std::sqrt(1 - (t1 * t1)) + (s * t2);

        // Reprojection onto Hemisphere
        StaticArray<float, 3> Nh = T1 * t1 + T2 * t2 + vh * (std::sqrt(std::max(0.f, 1.f - t1 * t1 - t2 * t2)));

        // Transform back to Ellipsoid config
        return (StaticArray<float, 3>(alphax * Nh.x(), alphay * Nh.y(), std::max<float>(1e-6f, Nh.z()))).Normalize();
    }
}