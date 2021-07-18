#ifndef _SHADING_CORE_H
#define _SHADING_CORE_H

#include "../maths/vec3_math.h"
#include "../maths/scalar.h"

namespace filianore
{

    inline float CosTheta(const StaticArray<float, 3> &w)
    {
        return w.z();
    }

    inline float Cos2Theta(const StaticArray<float, 3> &w)
    {
        return w.z() * w.z();
    }

    inline float AbsCosTheta(const StaticArray<float, 3> &w)
    {
        return std::abs(w.z());
    }

    inline float Sin2Theta(const StaticArray<float, 3> &w)
    {
        return std::max(0.f, 1.f - Cos2Theta(w));
    }

    inline float SinTheta(const StaticArray<float, 3> &w)
    {
        return std::sqrt(Sin2Theta(w));
    }

    inline float TanTheta(const StaticArray<float, 3> &w)
    {
        return SinTheta(w) / CosTheta(w);
    }

    inline float Tan2Theta(const StaticArray<float, 3> &w)
    {
        return Sin2Theta(w) / Cos2Theta(w);
    }

    inline float CosPhi(const StaticArray<float, 3> &w)
    {
        float sinTheta = SinTheta(w);
        return (sinTheta == 0.f) ? 1.f : Clamp<float>(w.x() / sinTheta, -1.f, 1.f);
    }

    inline float SinPhi(const StaticArray<float, 3> &w)
    {
        float sinTheta = SinTheta(w);
        return (sinTheta == 0.f) ? 0.f : Clamp<float>(w.y() / sinTheta, -1.f, 1.f);
    }

    inline float Cos2Phi(const StaticArray<float, 3> &w)
    {
        return CosPhi(w) * CosPhi(w);
    }

    inline float Sin2Phi(const StaticArray<float, 3> &w)
    {
        return SinPhi(w) * SinPhi(w);
    }

    inline bool SameHemisphere(const StaticArray<float, 3> &w, const StaticArray<float, 3> &wp)
    {
        return (w.z() * wp.z()) > 0;
    }

    inline StaticArray<float, 2> UniformSampleDisk(const StaticArray<float, 2> &u)
    {
        float r = std::sqrt(u.x());
        float theta = 2.f * Pi<float> * u.y();
        return StaticArray<float, 2>(r * std::cos(theta), r * std::sin(theta));
    }

    inline StaticArray<float, 2> ConcentricSampleDisk(const StaticArray<float, 2> &u)
    {
        // Map uniform random numbers to $[-1,1]^2$
        StaticArray<float, 2> uOffset = u * 2.f - StaticArray<float, 2>(1.f, 1.f);

        // Handle degeneracy at the origin
        if (uOffset.x() == 0 && uOffset.y() == 0)
            return StaticArray<float, 2>(0.f, 0.f);

        // Apply concentric mapping to point
        float theta, r;
        if (std::abs(uOffset.x()) > std::abs(uOffset.y()))
        {
            r = uOffset.x();
            theta = PiOver4<float> * (uOffset.y() / uOffset.x());
        }
        else
        {
            r = uOffset.y();
            theta = PiOver2<float> - PiOver4<float> * (uOffset.x() / uOffset.y());
        }
        return StaticArray<float, 2>(std::cos(theta), std::sin(theta)) * r;
    }

    inline StaticArray<float, 3> CosineHemisphereSample(const StaticArray<float, 2> &u)
    {
        StaticArray<float, 2> d = ConcentricSampleDisk(u);
        float z = std::sqrt(std::max(0.f, 1.f - d.x() * d.x() - d.y() * d.y()));
        return StaticArray<float, 3>(d.x(), d.y(), z);
    }

    inline float CosineHemispherePdf(float cosTheta)
    {
        return cosTheta * InvPi<float>;
    }

    inline StaticArray<float, 3> UniformSampleHemisphere(const StaticArray<float, 2> &u)
    {
        float z = u.params[0];
        float r = std::sqrt(std::max(0.f, 1.f - z * z));
        float phi = 2.f * Pi<float> * u.params[1];
        return StaticArray<float, 3>(r * std::cos(phi), r * std::sin(phi), z);
    }

    inline float UniformHemispherePdf()
    {
        return Inv2Pi<float>;
    }
} // namespace filianore

#endif