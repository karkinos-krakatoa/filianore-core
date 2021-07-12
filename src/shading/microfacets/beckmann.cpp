#include "filianore/shading/microfacets/beckmann.h"
#include "filianore/core/shadingcore.h"
#include "filianore/maths/vec3_math.h"

namespace filianore
{

    BeckmannDistribution::BeckmannDistribution(float _alphax, float _alphay)
        : alphax(std::max(0.001f, _alphax)),
          alphay(std::max(0.001f, _alphay))
    {
    }

    float BeckmannDistribution::EvaluateD(const StaticArray<float, 3> &wh) const
    {
        float tan2Theta = Tan2Theta(wh);
        if (std::isinf(tan2Theta))
            return 0.f;
        float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
        return std::exp(-tan2Theta * (Cos2Phi(wh) / (alphax * alphax) +
                                      Sin2Phi(wh) / (alphay * alphay))) /
               (Pi<float> * alphax * alphay * cos4Theta);
    }

    float BeckmannDistribution::Lambda(const StaticArray<float, 3> &w) const
    {
        float absTanTheta = std::abs(TanTheta(w));
        if (std::isinf(absTanTheta))
            return 0.f;
        float alpha = std::sqrt(Cos2Phi(w) * alphax * alphax +
                                Sin2Phi(w) * alphay * alphay);

        float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
        return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
    }

    StaticArray<float, 3> BeckmannDistribution::SampleWh(const StaticArray<float, 3> &wo, const StaticArray<float, 2> &u) const
    {
        float tan2Theta, phi;
        if (alphax == alphay)
        {
            float logSample = std::log(1 - u.x());
            tan2Theta = -alphax * alphax * logSample;
            phi = u.y() * 2.f * Pi<float>;
        }
        else
        {
            float logSample = std::log(1 - u.x());
            phi = std::atan(alphay / alphax * std::tan(2 * Pi<float> * u.y() + 0.5f * Pi<float>));
            if (u.y() > 0.5f)
                phi += Pi<float>;
            float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
            tan2Theta = -logSample /
                        (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
        }

        float cosTheta = 1 / std::sqrt(1 + tan2Theta);
        float sinTheta = std::sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
        StaticArray<float, 3> wh = SphericalDirection<float>(sinTheta, cosTheta, phi);
        if (!SameHemisphere(wo, wh))
            wh = wh.Neg();
        return wh;
    }
}