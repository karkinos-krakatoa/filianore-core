#ifndef _PRINCIPAL_SPECTRUM_H
#define _PRINCIPAL_SPECTRUM_H

#include "basespectrum.h"

namespace filianore
{
    static const int sampledWavelengthStart = 400;
    static const int sampledWavelengthEnd = 700;
    static const int nSpectralSamples = 60;

    class PrincipalSpectrum : public BaseSpectrum<nSpectralSamples>
    {
    public:
        PrincipalSpectrum(float v = 0.f)
            : BaseSpectrum(v) {}

        PrincipalSpectrum(const BaseSpectrum<nSpectralSamples> &v)
            : BaseSpectrum<nSpectralSamples>(v) {}
    };
}

#endif