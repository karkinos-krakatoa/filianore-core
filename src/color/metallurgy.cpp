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

            CopperEta.c[i] = AverageSpectrumSamples(Copper_Lambda, Copper_Eta, Copper_nSamples, wl0, wl1);
            CopperK.c[i] = AverageSpectrumSamples(Copper_Lambda, Copper_K, Copper_nSamples, wl0, wl1);
        }
    }

    std::pair<PrincipalSpectrum, PrincipalSpectrum> GetMetalCoefficientsFromForge(int value)
    {
        switch (value)
        {
        case 0:
            return std::make_pair(GoldEta, GoldK);
            break;
        case 1:
            return std::make_pair(CopperEta, CopperK);
            break;
        default:
            return std::make_pair(GoldEta, GoldK);
            break;
        }
    }

}