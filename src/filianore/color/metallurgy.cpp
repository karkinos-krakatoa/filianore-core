#include "metallurgy.h"
#include "cie.h"
#include "spectrumoperations.h"

namespace filianore {

void InitializeForge() {
    // Gold
    for (int i = 0; i < nSpectralSamples; ++i) {
        float wl0 = lerp<float>(float(i) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
        float wl1 = lerp<float>(float(i + 1) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);

        GoldEta.c[i] = average_spectrum_samples(Gold_Lambda, Gold_Eta, Gold_nSamples, wl0, wl1);
        GoldK.c[i] = average_spectrum_samples(Gold_Lambda, Gold_K, Gold_nSamples, wl0, wl1);

        CopperEta.c[i] = average_spectrum_samples(Copper_Lambda, Copper_Eta, Copper_nSamples, wl0, wl1);
        CopperK.c[i] = average_spectrum_samples(Copper_Lambda, Copper_K, Copper_nSamples, wl0, wl1);

        SilverEta.c[i] = average_spectrum_samples(Silver_Lambda, Silver_Eta, Silver_nSamples, wl0, wl1);
        SilverK.c[i] = average_spectrum_samples(Silver_Lambda, Silver_K, Silver_nSamples, wl0, wl1);

        AluminiumEta.c[i] = average_spectrum_samples(Aluminium_Lambda, Aluminium_Eta, Aluminium_nSamples, wl0, wl1);
        AluminiumK.c[i] = average_spectrum_samples(Aluminium_Lambda, Aluminium_K, Aluminium_nSamples, wl0, wl1);

        BerylliumEta.c[i] = average_spectrum_samples(Beryllium_Lambda, Beryllium_Eta, Beryllium_nSamples, wl0, wl1);
        BerylliumK.c[i] = average_spectrum_samples(Beryllium_Lambda, Beryllium_K, Beryllium_nSamples, wl0, wl1);

        ChromiumEta.c[i] = average_spectrum_samples(Chromium_Lambda, Chromium_Eta, Chromium_nSamples, wl0, wl1);
        ChromiumK.c[i] = average_spectrum_samples(Chromium_Lambda, Chromium_K, Chromium_nSamples, wl0, wl1);

        MercuryEta.c[i] = average_spectrum_samples(Mercury_Lambda, Mercury_Eta, Mercury_nSamples, wl0, wl1);
        MercuryK.c[i] = average_spectrum_samples(Mercury_Lambda, Mercury_K, Mercury_nSamples, wl0, wl1);

        IridiumEta.c[i] = average_spectrum_samples(Iridium_Lambda, Iridium_Eta, Iridium_nSamples, wl0, wl1);
        IridiumK.c[i] = average_spectrum_samples(Iridium_Lambda, Iridium_K, Iridium_nSamples, wl0, wl1);

        LithiumEta.c[i] = average_spectrum_samples(Lithium_Lambda, Lithium_Eta, Lithium_nSamples, wl0, wl1);
        LithiumK.c[i] = average_spectrum_samples(Lithium_Lambda, Lithium_K, Lithium_nSamples, wl0, wl1);
    }
}

std::pair<PrincipalSpectrum, PrincipalSpectrum> GetMetalCoefficientsFromForge(int value) {
    switch (value) {
    case 0:
        return std::make_pair(GoldEta, GoldK);
        break;
    case 1:
        return std::make_pair(CopperEta, CopperK);
        break;
    case 2:
        return std::make_pair(SilverEta, SilverK);
        break;
    case 3:
        return std::make_pair(AluminiumEta, AluminiumK);
        break;
    case 4:
        return std::make_pair(BerylliumEta, BerylliumK);
        break;
    case 5:
        return std::make_pair(ChromiumEta, ChromiumK);
        break;
    case 6:
        return std::make_pair(MercuryEta, MercuryK);
        break;
    case 7:
        return std::make_pair(IridiumEta, IridiumK);
        break;
    case 8:
        return std::make_pair(LithiumEta, LithiumK);
        break;
    default:
        return std::make_pair(GoldEta, GoldK);
        break;
    }
}

} // namespace filianore