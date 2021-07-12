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
        float e = (Cos2Phi(wh) / (alphax * alphax) + Sin2Phi(wh) / (alphay * alphay)) *
                  tan2Theta;
        return 1 / (Pi<float> * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
    }

    float GGXDistribution::Lambda(const StaticArray<float, 3> &w) const
    {
        float absTanTheta = std::abs(TanTheta(w));
        if (std::isinf(absTanTheta))
            return 0.;
        // Compute _alpha_ for direction _w_
        float alpha = std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
        float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
        return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
    }

    StaticArray<float, 3> GGXDistribution::SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const
    {
        StaticArray<float, 3> wh;
        float cosTheta = 0, phi = (2 * Pi<float>)*u.y();
        if (alphax == alphay)
        {
            float tanTheta2 = alphax * alphax * u.x() / (1.0f - u.x());
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        }
        else
        {
            phi =
                std::atan(alphay / alphax * std::tan(2 * Pi<float> * u.y() + .5f * Pi<float>));
            if (u.y() > .5f)
                phi += Pi<float>;
            float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            const float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
            const float alpha2 =
                1 / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
            float tanTheta2 = alpha2 * u.x() / (1 - u.x());
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        }
        float sinTheta =
            std::sqrt(std::max((float)0., (float)1. - cosTheta * cosTheta));

        wh = SphericalDirection<float>(sinTheta, cosTheta, phi);

        if (!SameHemisphere(wo, wh))
            wh = wh.Neg();
        return wh;
    }
}