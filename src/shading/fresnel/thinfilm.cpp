#include "filianore/shading/fresnel/thinfilm.h"

#include <iostream>
#include <algorithm>

namespace filianore
{

    PrincipalSpectrum ThinFilmInteference::Evaluate(float cosThetaI) const
    {
        // Thin film angle cosines
        float sinSqrd1 = ((extIOR / thinFilmIOR) / (extIOR / thinFilmIOR)) * (1 - cosThetaI * cosThetaI);
        float sinSqrd2 = ((extIOR / mediumIOR) / (extIOR / mediumIOR)) * (1 - cosThetaI * cosThetaI);
        if ((sinSqrd1 > 1) || (sinSqrd2 > 1))
        {
            return PrincipalSpectrum(1.f);
        }

        float cos1 = std::sqrt(1 - sinSqrd1);
        float cos2 = std::sqrt(1 - sinSqrd2);

        // Fresnel Amplitude Coefficients
        float alphaSP = SPAmpReflectionCoeff(thinFilmIOR, extIOR, cos1, cosThetaI) * SPAmpReflectionCoeff(mediumIOR, thinFilmIOR, cos1, cos2);
        float alphaPP = PPAmpReflectionCoeff(thinFilmIOR, extIOR, cos1, cosThetaI) * PPAmpReflectionCoeff(mediumIOR, thinFilmIOR, cos1, cos2);
        float betaSP = SPAmpTransmissionCoeff(extIOR, thinFilmIOR, cosThetaI, cos1) * SPAmpTransmissionCoeff(thinFilmIOR, mediumIOR, cos1, cos2);
        float betaPP = PPAmpTransmissionCoeff(extIOR, thinFilmIOR, cosThetaI, cos1) * PPAmpTransmissionCoeff(thinFilmIOR, mediumIOR, cos1, cos2);

        // Reflection Phase Change
        float delta10 = thinFilmIOR < extIOR ? Pi<float> : 0.f;
        float delta12 = thinFilmIOR < mediumIOR ? Pi<float> : 0.f;
        float delta = delta10 + delta12;

        auto TransmissionCoeff = [cos1, delta](float wavelength, float alphaCoeff, float betaCoeff, float thinFilmIOR, float thickness)
        {
            float phi = (2.f * Pi<float> / wavelength) * (2.f * thinFilmIOR * thickness * cos1) + delta;
            return (betaCoeff * betaCoeff) / ((alphaCoeff * alphaCoeff) - (2.f * alphaCoeff * std::cos(phi)) + 1.f);
        };

        // S- and P-Polarized Intensity Transmission Coefficient
        std::vector<float> tss, tpp;
        tss.reserve(nSpectralSamples);
        tpp.reserve(nSpectralSamples);

        for (int i = 400; i < 700; i = i + 5)
        {
            float ts = TransmissionCoeff(i, alphaSP, betaSP, thinFilmIOR, thickness);
            tss.emplace_back(ts);

            float tp = TransmissionCoeff(i, alphaPP, betaPP, thinFilmIOR, thickness);
            tpp.emplace_back(tp);
        }

        PrincipalSpectrum Ts = PrincipalSpectrum(tss);
        PrincipalSpectrum Tp = PrincipalSpectrum(tpp);

        // Transmitted Power Ratio for Medium Change
        float beamRatio = (mediumIOR * cos2) / (extIOR * cosThetaI);

        // Return Reflectance
        return PrincipalSpectrum(1.f) - (Ts + Tp) * 0.5f * beamRatio;
    }

} // namespace filianore