#include "filianore/color/metallurgy.h"
#include "filianore/color/spectrumoperations.h"
#include "filianore/color/cie.h"

namespace filianore
{

    void InitializeForge()
    {
        // Gold
        for (int i = 0; i < nSpectralSamples; ++i)
        {
            float wl0 = Lerp<float>(float(i) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
            float wl1 = Lerp<float>(float(i + 1) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);

            GoldEta.c[i] = AverageSpectrumSamples(Gold_Lambda, Gold_Eta, Gold_nSamples, wl0, wl1);
            GoldK.c[i] = AverageSpectrumSamples(Gold_Lambda, Gold_K, Gold_nSamples, wl0, wl1);
        }
    }

    std::pair<PrincipalSpectrum, PrincipalSpectrum> GetMetalCoefficientsFromForge(int value)
    {
        switch (value)
        {
        case 0:
            return std::make_pair(GoldEta, GoldK);
            break;
        default:
            return std::make_pair(GoldEta, GoldK);
            break;
        }
    }

}