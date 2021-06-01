#include "filianore/shading/beckmann.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    inline float Erf(float x)
    {
        // constants
        float a1 = 0.254829592f;
        float a2 = -0.284496736f;
        float a3 = 1.421413741f;
        float a4 = -1.453152027f;
        float a5 = 1.061405429f;
        float p = 0.3275911f;

        // Save the sign of x
        int sign = 1;
        if (x < 0)
            sign = -1;
        x = std::abs(x);

        // A&S formula 7.1.26
        float t = 1 / (1 + p * x);
        float y =
            1 -
            (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);

        return sign * y;
    }

    static float ErfInv(float x)
    {
        float w, p;
        x = Clamp<float>(x, -.99999f, .99999f);
        w = -std::log((1 - x) * (1 + x));
        if (w < 5)
        {
            w = w - 2.5f;
            p = 2.81022636e-08f;
            p = 3.43273939e-07f + p * w;
            p = -3.5233877e-06f + p * w;
            p = -4.39150654e-06f + p * w;
            p = 0.00021858087f + p * w;
            p = -0.00125372503f + p * w;
            p = -0.00417768164f + p * w;
            p = 0.246640727f + p * w;
            p = 1.50140941f + p * w;
        }
        else
        {
            w = std::sqrt(w) - 3;
            p = -0.000200214257f;
            p = 0.000100950558f + p * w;
            p = 0.00134934322f + p * w;
            p = -0.00367342844f + p * w;
            p = 0.00573950773f + p * w;
            p = -0.0076224613f + p * w;
            p = 0.00943887047f + p * w;
            p = 1.00167406f + p * w;
            p = 2.83297682f + p * w;
        }
        return p * x;
    }

    float BeckmannDistribution::EvaluateD(const StaticArray<float, 3> &wh) const
    {
        float tan2Theta = Tan2Theta(wh);
        if (std::isinf(tan2Theta))
            return 0.;
        float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
        return std::exp(-tan2Theta * (Cos2Phi(wh) / (alphax * alphax) +
                                      Sin2Phi(wh) / (alphay * alphay))) /
               (Pi<float> * alphax * alphay * cos4Theta);
    }

    float BeckmannDistribution::Lambda(const StaticArray<float, 3> &w) const
    {
        float absTanTheta = std::abs(TanTheta(w));
        if (std::isinf(absTanTheta))
            return 0.;
        // Compute _alpha_ for direction _w_
        float alpha =
            std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
        float a = 1 / (alpha * absTanTheta);
        if (a >= 1.6f)
            return 0;
        return (1 - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
    }

    static void BeckmannSample11(float cosThetaI, float U1, float U2,
                                 float *slope_x, float *slope_y)
    {
        /* Special case (normal incidence) */
        if (cosThetaI > .9999)
        {
            float r = std::sqrt(-std::log(1.0f - U1));
            float sinPhi = std::sin(2 * Pi<float> * U2);
            float cosPhi = std::cos(2 * Pi<float> * U2);
            *slope_x = r * cosPhi;
            *slope_y = r * sinPhi;
            return;
        }

        /* The original inversion routine from the paper contained
       discontinuities, which causes issues for QMC integration
       and techniques like Kelemen-style MLT. The following code
       performs a numerical inversion with better behavior */
        float sinThetaI =
            std::sqrt(std::max((float)0, (float)1 - cosThetaI * cosThetaI));
        float tanThetaI = sinThetaI / cosThetaI;
        float cotThetaI = 1 / tanThetaI;

        /* Search interval -- everything is parameterized
       in the Erf() domain */
        float a = -1, c = Erf(cotThetaI);
        float sample_x = std::max(U1, (float)1e-6f);

        /* Start with a good initial guess */
        // float b = (1-sample_x) * a + sample_x * c;

        /* We can do better (inverse of an approximation computed in
     * Mathematica) */
        float thetaI = std::acos(cosThetaI);
        float fit = 1 + thetaI * (-0.876f + thetaI * (0.4265f - 0.0594f * thetaI));
        float b = c - (1 + c) * std::pow(1 - sample_x, fit);

        /* Normalization factor for the CDF */
        static const float SQRT_PI_INV = 1.f / std::sqrt(Pi<float>);
        float normalization =
            1 /
            (1 + c + SQRT_PI_INV * tanThetaI * std::exp(-cotThetaI * cotThetaI));

        int it = 0;
        while (++it < 10)
        {
            /* Bisection criterion -- the oddly-looking
           Boolean expression are intentional to check
           for NaNs at little additional cost */
            if (!(b >= a && b <= c))
                b = 0.5f * (a + c);

            /* Evaluate the CDF and its derivative
           (i.e. the density function) */
            float invErf = ErfInv(b);
            float value =
                normalization *
                    (1 + b + SQRT_PI_INV * tanThetaI * std::exp(-invErf * invErf)) -
                sample_x;
            float derivative = normalization * (1 - invErf * tanThetaI);

            if (std::abs(value) < 1e-5f)
                break;

            /* Update bisection intervals */
            if (value > 0)
                c = b;
            else
                a = b;

            b -= value / derivative;
        }

        /* Now convert back into a slope value */
        *slope_x = ErfInv(b);

        /* Simulate Y component */
        *slope_y = ErfInv(2.0f * std::max(U2, (float)1e-6f) - 1.0f);
    }

    static StaticArray<float, 3> BeckmannSample(const StaticArray<float, 3> &wi, float alpha_x, float alpha_y,
                                                float U1, float U2)
    {
        // 1. stretch wi
        StaticArray<float, 3> wiStretched =
            (StaticArray<float, 3>(alpha_x * wi.x(), alpha_y * wi.y(), wi.z())).Normalize();

        // 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
        float slope_x, slope_y;
        BeckmannSample11(CosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

        // 3. rotate
        float tmp = CosPhi(wiStretched) * slope_x - SinPhi(wiStretched) * slope_y;
        slope_y = SinPhi(wiStretched) * slope_x + CosPhi(wiStretched) * slope_y;
        slope_x = tmp;

        // 4. unstretch
        slope_x = alpha_x * slope_x;
        slope_y = alpha_y * slope_y;

        // 5. compute normal
        return (StaticArray<float, 3>(-slope_x, -slope_y, 1.f)).Normalize();
    }

    StaticArray<float, 3> BeckmannDistribution::SampleWh(const StaticArray<float, 3> &wo,
                                                         const StaticArray<float, 2> &u) const
    {
        if (!sampleVisibleArea)
        {
            // Sample full distribution of normals for Beckmann distribution

            // Compute $\tan^2 \theta$ and $\phi$ for Beckmann distribution sample
            float tan2Theta, phi;
            if (alphax == alphay)
            {
                float logSample = std::log(1 - u.params[0]);
                tan2Theta = -alphax * alphax * logSample;
                phi = u.params[1] * 2 * Pi<float>;
            }
            else
            {
                // Compute _tan2Theta_ and _phi_ for anisotroPi<float>c Beckmann
                // distribution
                float logSample = std::log(1 - u.params[0]);
                phi = std::atan(alphay / alphax *
                                std::tan(2 * Pi<float> * u.params[1] + 0.5f * Pi<float>));
                if (u.params[1] > 0.5f)
                    phi += Pi<float>;
                float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
                float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
                tan2Theta = -logSample /
                            (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
            }

            // Map sampled Beckmann angles to normal direction _wh_
            float cosTheta = 1 / std::sqrt(1 + tan2Theta);
            float sinTheta = std::sqrt(std::max((float)0, 1 - cosTheta * cosTheta));
            StaticArray<float, 3> wh = SphericalDirection<float>(sinTheta, cosTheta, phi);
            if (!SameHemisphere(wo, wh))
                wh = wh.Neg();
            return wh;
        }
        else
        {
            // Sample visible area of normals for Beckmann distribution
            StaticArray<float, 3> wh;
            bool flip = wo.z() < 0;
            StaticArray<float, 3> woN = wo;
            wh = BeckmannSample(flip ? woN.Neg() : wo, alphax, alphay, u.params[0], u.params[1]);
            if (flip)
                wh = wh.Neg();
            return wh;
        }
    }
}