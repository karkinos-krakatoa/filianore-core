#ifndef _SPECTRUM_INITS_H
#define _SPECTRUM_INITS_H

#include "principalspectrum.h"

namespace filianore
{
    inline PrincipalSpectrum X, Y, Z;
    inline PrincipalSpectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
    inline PrincipalSpectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
    inline PrincipalSpectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
    inline PrincipalSpectrum rgbRefl2SpectBlue;
    inline PrincipalSpectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
    inline PrincipalSpectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
    inline PrincipalSpectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
    inline PrincipalSpectrum rgbIllum2SpectBlue;

    void InitializeSpectrals();
}

#endif