#ifndef _SAMPLING_H
#define _SAMPLING_H

#include <algorithm>
#include "elemental.h"
#include "../maths/vec3_math.h"
#include "../maths/scalar.h"
#include "../ext/pcg32.h"

namespace filianore
{

    FILIANORE_INLINE void Shuffle(float *samp, int count, int nDimensions, pcg32 &pcg32);

    FILIANORE_INLINE float BalanceHeuristic(int nf, float fPdf, int ng, float gPdf);

    FILIANORE_INLINE float PowerHeuristic(int nf, float fPdf, int ng, float gPdf);

    FILIANORE_INLINE float GenericRandNumber();

    // FILIANORE_INLINE StaticArray<float, 2> UniformSampleDisk(const StaticArray<float, 2> &u);

    // FILIANORE_INLINE StaticArray<float, 3> UniformSampleHemisphere(const StaticArray<float, 2> &u);

    // FILIANORE_INLINE float UniformHemispherePdf();

    // FILIANORE_INLINE StaticArray<float, 2> ConcentricSampleDisk(const StaticArray<float, 2> &u);

    // FILIANORE_INLINE StaticArray<float, 3> CosineSampleHemisphere(const StaticArray<float, 2> &u);

    // FILIANORE_INLINE float CosineHemispherePdf(float cosTheta);

} // namespace filianore

#endif