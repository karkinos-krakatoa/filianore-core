#ifndef _SPECTRUM_OPERATIONS_H
#define _SPECTRUM_OPERATIONS_H

#include "../maths/vec.h"
#include "principalspectrum.h"

namespace filianore {
namespace GammaCorrectionType {
enum GammaCorrectionType {
    NONE = 1,
    REC_709 = 2,
    GAMMA_1_8 = 3,
    GAMMA_2_2 = 4,
    GAMMA_4_0 = 5
};
}

namespace ToneMappingType {
enum ToneMappingType {
    NONE = 1,
    ACES_BASIC = 2,
    REINHARD = 3,
    FILMIC = 4
};
}

extern bool spectrum_samples_sorted(const float *wavelengths, const float *values, int n);
extern void sort_spectrum_samples(float *wavelengths, float *values, int n);
extern float average_spectrum_samples(const float *wavelength, const float *vals, int n, float wavelengthStart, float wavelengthEnd);

Vector3f xyz_to_rgb(const Vector3f &xyz);
Vector3f xyz_to_aces2065_1(const Vector3f &xyz);

PrincipalSpectrum from_spd(const float *wavelengths, const float *values, int n);
PrincipalSpectrum from_spd_exact(const float *values);
PrincipalSpectrum from_reflectance_rgb(const Vector3f &rgb);
PrincipalSpectrum from_illuminance_rgb(const Vector3f &rgb);

Vector3f to_xyz(const PrincipalSpectrum &r);
Vector3f to_rgb(const PrincipalSpectrum &r);

Vector3f gamma_correct(Vector3f &inputColor, int gammaCorrectionType);
Vector3f tone_map(Vector3f &inputColor, int tonemapType);

float luminance(const PrincipalSpectrum &r);
} // namespace filianore

#endif