#include "filianore/shading/fresnel/thinfilm.h"

#include <algorithm>
#include <iostream>

namespace filianore {

PrincipalSpectrum ThinFilmInteference::evaluate(float cosThetaI) const {
    // Thin film angle cosines
    float sinSqrd1 = ((extIOR / thinFilmIOR) / (extIOR / thinFilmIOR)) * (1 - cosThetaI * cosThetaI);
    float sinSqrd2 = ((extIOR / mediumIOR) / (extIOR / mediumIOR)) * (1 - cosThetaI * cosThetaI);
    if ((sinSqrd1 > 1) || (sinSqrd2 > 1)) {
        return PrincipalSpectrum(1.f);
    }

    float cos1 = std::sqrt(1 - sinSqrd1);
    float cos2 = std::sqrt(1 - sinSqrd2);

    // Fresnel Amplitude Coefficients
    float alphaSP = sp_amp_reflection_coefficient(thinFilmIOR, extIOR, cos1, cosThetaI) * sp_amp_reflection_coefficient(mediumIOR, thinFilmIOR, cos1, cos2);
    float alphaPP = pp_amp_reflection_coefficient(thinFilmIOR, extIOR, cos1, cosThetaI) * pp_amp_reflection_coefficient(mediumIOR, thinFilmIOR, cos1, cos2);
    float betaSP = sp_amp_transmission_coefficient(extIOR, thinFilmIOR, cosThetaI, cos1) * sp_amp_transmission_coefficient(thinFilmIOR, mediumIOR, cos1, cos2);
    float betaPP = pp_amp_transmission_coefficient(extIOR, thinFilmIOR, cosThetaI, cos1) * pp_amp_transmission_coefficient(thinFilmIOR, mediumIOR, cos1, cos2);

    // Reflection Phase Change
    float delta10 = thinFilmIOR < extIOR ? PI : 0.f;
    float delta12 = thinFilmIOR < mediumIOR ? PI : 0.f;
    float delta = delta10 + delta12;

    auto TransmissionCoeff = [cos1, delta](float wavelength, float alphaCoeff, float betaCoeff, float thinFilmIOR, float thickness) {
        float phi = (2.f * PI / wavelength) * (2.f * thinFilmIOR * thickness * cos1) + delta;
        return (betaCoeff * betaCoeff) / ((alphaCoeff * alphaCoeff) - (2.f * alphaCoeff * std::cos(phi)) + 1.f);
    };

    // S- and P-Polarized Intensity Transmission Coefficient
    std::vector<float> tss, tpp;
    tss.reserve(nSpectralSamples);
    tpp.reserve(nSpectralSamples);

    for (int i = 400; i < 700; i = i + 5) {
        float ts = TransmissionCoeff(i, alphaSP, betaSP, thinFilmIOR, thickness);
        tss.emplace_back(ts);

        float tp = TransmissionCoeff(i, alphaPP, betaPP, thinFilmIOR, thickness);
        tpp.emplace_back(tp);
    }

    PrincipalSpectrum Ts = PrincipalSpectrum(tss);
    PrincipalSpectrum Tp = PrincipalSpectrum(tpp);

    // Transmitted power Ratio for Medium Change
    float beamRatio = (mediumIOR * cos2) / (extIOR * cosThetaI);

    // Return Reflectance
    return PrincipalSpectrum(1.f) - (Ts + Tp) * 0.5f * beamRatio;
}

} // namespace filianore