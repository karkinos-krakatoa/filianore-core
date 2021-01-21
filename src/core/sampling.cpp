#include "filianore/core/sampling.h"

namespace filianore
{

    FILIANORE_INLINE void Shuffle(float *samp, int count, int nDimensions, pcg32 &pcg32)
    {
        for (int i = 0; i < count; ++i)
        {
            int other = i + pcg32.nextUInt(count - i);
            for (int j = 0; j < nDimensions; ++j)
            {
                std::swap(samp[nDimensions * i + j], samp[nDimensions * other + j]);
            }
        }
    }

    FILIANORE_INLINE float BalanceHeuristic(int nf, float fPdf, int ng, float gPdf)
    {
        return (nf * fPdf) / (nf * fPdf + ng * gPdf);
    }

    FILIANORE_INLINE float PowerHeuristic(int nf, float fPdf, int ng, float gPdf)
    {
        float f = nf * fPdf;
        float g = ng * gPdf;

        return (f * f) / (f * f + g * g);
    }

    FILIANORE_INLINE float GenericRandNumber()
    {
        return (float)rand() / ((float)RAND_MAX);
    }

    // FILIANORE_INLINE StaticArray<float, 2> UniformSampleDisk(const StaticArray<float, 2> &u)
    // {
    //     float r = std::sqrt(u.x());
    //     float theta = 2.f * Pi<float> * u.y();
    //     return StaticArray<float, 2>(r * std::cos(theta), r * std::sin(theta));
    // }

    // FILIANORE_INLINE StaticArray<float, 3> UniformSampleHemisphere(const StaticArray<float, 2> &u)
    // {
    //     float z = u.x();
    //     float srtTerm = std::sqrt(std::max(0.f, 1.f - z * z));
    //     float brckTerm = 2.f * Pi<float> * u.y();

    //     return StaticArray<float, 3>(std::cos(brckTerm) * srtTerm, std::sin(brckTerm) * srtTerm, z);
    // }

    // FILIANORE_INLINE float UniformHemispherePdf()
    // {
    //     return Inv2Pi<float>;
    // }

    // FILIANORE_INLINE StaticArray<float, 2> ConcentricSampleDisk(const StaticArray<float, 2> &u)
    // {
    //     StaticArray<float, 2> uOffset = u * 2.f - StaticArray<float, 2>(1.f);

    //     if (uOffset.x() == 0 && uOffset.y() == 0)
    //     {
    //         return StaticArray<float, 2>(0.f);
    //     }

    //     float theta, r;
    //     if (std::abs(uOffset.x()) > std::abs(uOffset.y()))
    //     {
    //         r = uOffset.x();
    //         theta = PiOver4<float> * (uOffset.y() / uOffset.x());
    //     }
    //     else
    //     {
    //         r = uOffset.y();
    //         theta = PiOver2<float> - PiOver4<float> * (uOffset.y() / uOffset.x());
    //     }

    //     return StaticArray<float, 2>(std::cos(theta), std::sin(theta)) * r;
    // }

    // FILIANORE_INLINE StaticArray<float, 3> CosineSampleHemisphere(const StaticArray<float, 2> &u)
    // {
    //     StaticArray<float, 2> d = ConcentricSampleDisk(u);
    //     float z = std::sqrt(std::max(0.f, 1.f - d.x() * d.x() - d.y() * d.y()));
    //     return StaticArray<float, 3>(d.x(), d.y(), z);
    // }

    // FILIANORE_INLINE float CosineHemispherePdf(float cosTheta)
    // {
    //     return cosTheta * InvPi<float>;
    // }
}