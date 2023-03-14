#include "filianore/color/spectruminits.h"
#include "filianore/color/cie.h"
#include "filianore/color/spectrumoperations.h"

namespace filianore {
void InitializeSpectrals() {
    // Compute X, Y, Z
    for (int i = 0; i < nSpectralSamples; ++i) {
        float wl0 = lerp<float>(float(i) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
        float wl1 = lerp<float>(float(i + 1) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);

        X.c[i] = average_spectrum_samples(CIE_Wavelength, CIE_X, nCIESamples, wl0, wl1);
        Y.c[i] = average_spectrum_samples(CIE_Wavelength, CIE_Y, nCIESamples, wl0, wl1);
        Z.c[i] = average_spectrum_samples(CIE_Wavelength, CIE_Z, nCIESamples, wl0, wl1);
    }

    // Compute RGB to spectrum functions
    for (int i = 0; i < nSpectralSamples; ++i) {
        float wl0 = lerp<float>(float(i) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
        float wl1 = lerp<float>(float(i + 1) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
        rgbRefl2SpectWhite.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBRefl2SpectWhite,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbRefl2SpectCyan.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBRefl2SpectCyan,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbRefl2SpectMagenta.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBRefl2SpectMagenta,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbRefl2SpectYellow.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBRefl2SpectYellow,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbRefl2SpectRed.c[i] = average_spectrum_samples(
            RGB2SpectLambda, RGBRefl2SpectRed, nRGB2SpectSamples, wl0, wl1);
        rgbRefl2SpectGreen.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBRefl2SpectGreen,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbRefl2SpectBlue.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBRefl2SpectBlue,
                                     nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectWhite.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectWhite,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbIllum2SpectCyan.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectCyan,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbIllum2SpectMagenta.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectMagenta,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbIllum2SpectYellow.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectYellow,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbIllum2SpectRed.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectRed,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbIllum2SpectGreen.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectGreen,
                                     nRGB2SpectSamples, wl0, wl1);
        rgbIllum2SpectBlue.c[i] =
            average_spectrum_samples(RGB2SpectLambda, RGBIllum2SpectBlue,
                                     nRGB2SpectSamples, wl0, wl1);
    }
}
} // namespace filianore