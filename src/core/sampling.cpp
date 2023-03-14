#include "filianore/core/sampling.h"

namespace filianore {

FILIANORE_INLINE void shuffle(float *samp, int count, int nDimensions, pcg32 &pcg32) {
    for (int i = 0; i < count; ++i) {
        int other = i + pcg32.nextUInt(count - i);
        for (int j = 0; j < nDimensions; ++j) {
            std::swap(samp[nDimensions * i + j], samp[nDimensions * other + j]);
        }
    }
}

FILIANORE_INLINE float balance_heuristic(int nf, float fPdf, int ng, float gPdf) {
    return (nf * fPdf) / (nf * fPdf + ng * gPdf);
}

FILIANORE_INLINE float power_heuristic(int nf, float fPdf, int ng, float gPdf) {
    float f = nf * fPdf;
    float g = ng * gPdf;

    return (f * f) / (f * f + g * g);
}

FILIANORE_INLINE float generic_rand_number() {
    return (float)rand() / ((float)RAND_MAX);
}

// FILIANORE_INLINE Vector2f uniform_sample_disk(const Vector2f &u)
// {
//     float r = std::sqrt(u.x);
//     float theta = 2.f * PI * u.y;
//     return Vector2f(r * std::cos(theta), r * std::sin(theta));
// }

// FILIANORE_INLINE Vector3f uniform_sample_hemisphere(const Vector2f &u)
// {
//     float z = u.x;
//     float srtTerm = std::sqrt(std::max(0.f, 1.f - z * z));
//     float brckTerm = 2.f * PI * u.y;

//     return Vector3f(std::cos(brckTerm) * srtTerm, std::sin(brckTerm) * srtTerm, z);
// }

// FILIANORE_INLINE float uniform_hemisphere_pdf()
// {
//     return Inv2PI;
// }

// FILIANORE_INLINE Vector2f concentric_sample_disk(const Vector2f &u)
// {
//     Vector2f uOffset = u * 2.f - Vector2f(1.f);

//     if (uOffset.x == 0 && uOffset.y == 0)
//     {
//         return Vector2f(0.f);
//     }

//     float theta, r;
//     if (std::abs(uOffset.x) > std::abs(uOffset.y))
//     {
//         r = uOffset.x;
//         theta = PiOver4<float> * (uOffset.y / uOffset.x);
//     }
//     else
//     {
//         r = uOffset.y;
//         theta = PiOver2<float> - PiOver4<float> * (uOffset.y / uOffset.x);
//     }

//     return Vector2f(std::cos(theta), std::sin(theta)) * r;
// }

// FILIANORE_INLINE Vector3f CosineSampleHemisphere(const Vector2f &u)
// {
//     Vector2f d = concentric_sample_disk(u);
//     float z = std::sqrt(std::max(0.f, 1.f - d.x * d.x - d.y * d.y));
//     return Vector3f(d.x, d.y, z);
// }

// FILIANORE_INLINE float cosine_hemisphere_pdf(float cosTheta)
// {
//     return cosTheta * INV_PI;
// }
} // namespace filianore