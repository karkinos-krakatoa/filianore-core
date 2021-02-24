#ifndef _SPECTRUM_OPERATIONS_H
#define _SPECTRUM_OPERATIONS_H

#include "principalspectrum.h"
#include "../maths/static_array.h"

namespace filianore
{
    extern bool SpectrumSamplesSorted(const float *wavelengths, const float *values, int n);
    extern void SortSpectrumSamples(float *wavelengths, float *values, int n);
    extern float AverageSpectrumSamples(const float *wavelength, const float *vals, int n, float wavelengthStart, float wavelengthEnd);

    StaticArray<float, 3> XYZToRGB(const StaticArray<float, 3> &xyz);

    PrincipalSpectrum FromSPD(const float *wavelengths, const float *values, int n);
    PrincipalSpectrum FromReflectanceRGB(const StaticArray<float, 3> &rgb);
    PrincipalSpectrum FromIlluminanceRGB(const StaticArray<float, 3> &rgb);

    StaticArray<float, 3> ToXYZ(const PrincipalSpectrum &r);
    StaticArray<float, 3> ToRGB(const PrincipalSpectrum &r);

    StaticArray<float, 3> GammaCorrect(StaticArray<float, 3> &rgb);

    float Luminance(const PrincipalSpectrum &r);
}

#endif