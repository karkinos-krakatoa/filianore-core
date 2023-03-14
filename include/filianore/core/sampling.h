#ifndef _SAMPLING_H
#define _SAMPLING_H

#include "../ext/pcg32.h"
#include "../maths/mathutils.h"
#include "../maths/vec.h"
#include "elemental.h"
#include <algorithm>

namespace filianore {

FILIANORE_INLINE void shuffle(float *samp, int count, int nDimensions, pcg32 &pcg32);

FILIANORE_INLINE float balance_heuristic(int nf, float fPdf, int ng, float gPdf);

FILIANORE_INLINE float power_heuristic(int nf, float fPdf, int ng, float gPdf);

FILIANORE_INLINE float generic_rand_number();

// FILIANORE_INLINE Vector2f uniform_sample_disk(const Vector2f &u);

// FILIANORE_INLINE Vector3f uniform_sample_hemisphere(const Vector2f &u);

// FILIANORE_INLINE float uniform_hemisphere_pdf();

// FILIANORE_INLINE Vector2f concentric_sample_disk(const Vector2f &u);

// FILIANORE_INLINE Vector3f CosineSampleHemisphere(const Vector2f &u);

// FILIANORE_INLINE float cosine_hemisphere_pdf(float cosTheta);

} // namespace filianore

#endif