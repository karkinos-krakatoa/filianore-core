#ifndef _SPECTRUM_FOUNDATION_H
#define _SPECTRUM_FOUNDATION_H

#include "cie.h"
#include "../maths/static_array.h"
#include "../maths/scalar.h"

namespace filianore
{
    static const unsigned int WavelengthStart = 400;
    static const unsigned int WavelengthEnd = 700;
    static const unsigned int WavelengthSamplingRate = 5;
    static const unsigned int WavelengthSamplesSize = 60;

    static float wavelengthAvgs[WavelengthSamplesSize] = {};
    static float CIE_X_Sampled[WavelengthSamplesSize] = {};
    static float CIE_Y_Sampled[WavelengthSamplesSize] = {};
    static float CIE_Z_Sampled[WavelengthSamplesSize] = {};

    static void SortSpectrumSamples(float *lambda, float *vals, int n)
    {
        std::vector<std::pair<float, float>> sortVec;
        sortVec.reserve(n);

        for (int i = 0; i < n; ++i)
        {
            sortVec.push_back(std::make_pair(lambda[i], vals[i]));
        }

        std::sort(sortVec.begin(), sortVec.end());

        for (int i = 0; i < n; ++i)
        {
            lambda[i] = sortVec[i].first;
            vals[i] = sortVec[i].second;
        }
    }

    static float AverageSpectrumSamples(const std::vector<float> &lambda, const std::vector<float> &vals, int n, float lambdaStart, float lambdaEnd)
    {
        // Handle cases with out-of-bounds range or single sample only
        if (lambdaEnd <= lambda[0])
            return vals[0];
        if (lambdaStart >= lambda[n - 1])
            return vals[n - 1];
        if (n == 1)
            return vals[0];

        float sum = 0;
        // Add contributions of constant segments before/after samples
        if (lambdaStart < lambda[0])
            sum += vals[0] * (lambda[0] - lambdaStart);
        if (lambdaEnd > lambda[n - 1])
            sum += vals[n - 1] * (lambdaEnd - lambda[n - 1]);

        // Advance to first relevant wavelength segment
        int i = 0;
        while (lambdaStart > lambda[i + 1])
            ++i;

        // Loop over wavelength sample segments and add contributions
        auto interp = [lambda, vals](float w, int i) {
            return Lerp<float>((w - lambda[i]) / (lambda[i + 1] - lambda[i]), vals[i], vals[i + 1]);
        };

        for (; i + 1 < n && lambdaEnd >= lambda[i]; ++i)
        {
            float segLambdaStart = std::max(lambdaStart, lambda[i]);
            float segLambdaEnd = std::min(lambdaEnd, lambda[i + 1]);
            sum += 0.5f * (interp(segLambdaStart, i) + interp(segLambdaEnd, i)) * (segLambdaEnd - segLambdaStart);
        }

        return sum / (lambdaEnd - lambdaStart);
    }

    // To avoid recalculating the wavelength samples average value, Run at AppInit Stage
    static void InitWavelengthAverages()
    {
        int wavelength = WavelengthStart;
        for (unsigned int y = 0; y < WavelengthSamplesSize; y++)
        {
            int sum = 0;
            for (unsigned int i = wavelength; i < wavelength + WavelengthSamplingRate; i++)
            {
                sum += i;
            }

            wavelengthAvgs[y] = float(sum / WavelengthSamplingRate);
            wavelength += WavelengthSamplingRate;
        }
    }

    // Get Sampled Version of the Color-Matching Functions
    // Make sure to run this at the AppInit Stage
    static void InitCIESampled()
    {
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            float w0 = Lerp<float>(float(i) / float(WavelengthSamplesSize), WavelengthStart, WavelengthEnd);
            float w1 = Lerp<float>(float(i + 1) / float(WavelengthSamplesSize), WavelengthStart, WavelengthEnd);

            CIE_X_Sampled[i] = AverageSpectrumSamples(CIE_Lambda, CIE_X, nCIESamples, w0, w1);
            CIE_Y_Sampled[i] = AverageSpectrumSamples(CIE_Lambda, CIE_Y, nCIESamples, w0, w1);
            CIE_Z_Sampled[i] = AverageSpectrumSamples(CIE_Lambda, CIE_Z, nCIESamples, w0, w1);
        }
    }

    static StaticArray<float, 3> GammaCorrect(StaticArray<float, 3> &rgb)
    {
        if (rgb.x() >= 0.018f)
            rgb.params[0] = (1.099f * powf(rgb.x(), 0.45f)) - 0.099f;
        else
            rgb.params[0] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
        if (rgb.y() >= 0.018f)
            rgb.params[1] = (1.099f * powf(rgb.y(), 0.45f)) - 0.099f;
        else
            rgb.params[1] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
        if (rgb.z() >= 0.018f)
            rgb.params[2] = (1.099f * powf(rgb.z(), 0.45f)) - 0.099f;
        else
            rgb.params[2] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;

        // Return the gamma-corrected color
        return rgb;
    }

    static void GammaCorrectPixels(StaticArray<float, 3> *pixels, const int N)
    {
        for (int t = 0; t < N; ++t)
        {
            pixels[t] = GammaCorrect(pixels[t]);
        }
    }

} // namespace filianore

#endif