#ifndef _SPECTRUM_COMPLETE_H
#define _SPECTRUM_COMPLETE_H

#include "spectrum.h"

namespace filianore
{
    static Spectrum<float> rgbRefl2SpectWhite;
    static Spectrum<float> rgbRefl2SpectCyan;
    static Spectrum<float> rgbRefl2SpectMagenta;
    static Spectrum<float> rgbRefl2SpectYellow;
    static Spectrum<float> rgbRefl2SpectRed;
    static Spectrum<float> rgbRefl2SpectGreen;
    static Spectrum<float> rgbRefl2SpectBlue;
    static Spectrum<float> rgbIllum2SpectWhite;
    static Spectrum<float> rgbIllum2SpectCyan;
    static Spectrum<float> rgbIllum2SpectMagenta;
    static Spectrum<float> rgbIllum2SpectYellow;
    static Spectrum<float> rgbIllum2SpectRed;
    static Spectrum<float> rgbIllum2SpectGreen;
    static Spectrum<float> rgbIllum2SpectBlue;

    static Spectrum<float> SpectralReflectanceFromRGB(const StaticArray<float, 3> &rgb)
    {
        Spectrum<float> res;

        if (rgb.params[0] <= rgb.params[1] && rgb.params[0] <= rgb.params[2])
        {
            res += rgbRefl2SpectWhite * rgb.params[0];
            if (rgb.params[1] <= rgb.params[2])
            {
                res += rgbRefl2SpectCyan * (rgb.params[1] - rgb.params[0]);
                res += rgbRefl2SpectBlue * (rgb.params[2] - rgb.params[1]);
            }
            else
            {
                res += rgbRefl2SpectCyan * (rgb.params[2] - rgb.params[0]);
                res += rgbRefl2SpectGreen * (rgb.params[1] - rgb.params[2]);
            }
        }
        else if (rgb.params[1] <= rgb.params[0] && rgb.params[1] <= rgb.params[2])
        {
            res += rgbRefl2SpectWhite * rgb.params[1];
            if (rgb.params[0] <= rgb.params[2])
            {
                res += rgbRefl2SpectMagenta * (rgb.params[0] - rgb.params[1]);
                res += rgbRefl2SpectBlue * (rgb.params[2] - rgb.params[0]);
            }
            else
            {
                res += rgbRefl2SpectMagenta * (rgb.params[2] - rgb.params[1]);
                res += rgbRefl2SpectRed * (rgb.params[0] - rgb.params[2]);
            }
        }
        else
        {
            res += rgbRefl2SpectWhite * rgb.params[2];
            if (rgb.params[0] <= rgb.params[1])
            {
                res += rgbRefl2SpectYellow * (rgb.params[0] - rgb.params[2]);
                res += rgbRefl2SpectGreen * (rgb.params[1] - rgb.params[0]);
            }
            else
            {
                res += rgbRefl2SpectYellow * (rgb.params[1] - rgb.params[2]);
                res += rgbRefl2SpectRed * (rgb.params[0] - rgb.params[1]);
            }
        }

        res *= .94f;
        res = res.SpecClamp();

        return res;
    }

    static Spectrum<float> SpectralIlluminanceFromRGB(const StaticArray<float, 3> &rgb)
    {
        Spectrum<float> res;

        if (rgb.params[0] <= rgb.params[1] && rgb.params[0] <= rgb.params[2])
        {
            res += rgbIllum2SpectWhite * rgb.params[0];
            if (rgb.params[1] <= rgb.params[2])
            {
                res += rgbIllum2SpectCyan * (rgb.params[1] - rgb.params[0]);
                res += rgbIllum2SpectBlue * (rgb.params[2] - rgb.params[1]);
            }
            else
            {
                res += rgbIllum2SpectCyan * (rgb.params[2] - rgb.params[0]);
                res += rgbIllum2SpectGreen * (rgb.params[1] - rgb.params[2]);
            }
        }
        else if (rgb.params[1] <= rgb.params[0] && rgb.params[1] <= rgb.params[2])
        {
            res += rgbIllum2SpectWhite * rgb.params[1];
            if (rgb.params[0] <= rgb.params[2])
            {
                res += rgbIllum2SpectMagenta * (rgb.params[0] - rgb.params[1]);
                res += rgbIllum2SpectBlue * (rgb.params[2] - rgb.params[0]);
            }
            else
            {
                res += rgbIllum2SpectMagenta * (rgb.params[2] - rgb.params[1]);
                res += rgbIllum2SpectRed * (rgb.params[0] - rgb.params[2]);
            }
        }
        else
        {
            res += rgbIllum2SpectWhite * rgb.params[2];
            if (rgb.params[0] <= rgb.params[1])
            {
                res += rgbIllum2SpectYellow * (rgb.params[0] - rgb.params[2]);
                res += rgbIllum2SpectGreen * (rgb.params[1] - rgb.params[0]);
            }
            else
            {
                res += rgbIllum2SpectYellow * (rgb.params[1] - rgb.params[2]);
                res += rgbIllum2SpectRed * (rgb.params[0] - rgb.params[1]);
            }
        }

        res *= .86445f;
        res = res.SpecClamp();

        return res;
    }

    static void InitializeRGB2Spectrum()
    {
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            float w0 = Lerp<float>(float(i) / float(WavelengthSamplesSize), WavelengthStart, WavelengthEnd);
            float w1 = Lerp<float>(float(i + 1) / float(WavelengthSamplesSize), WavelengthStart, WavelengthEnd);

            rgbRefl2SpectWhite.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectWhite, nRGB2SpectSamples, w0, w1);
            rgbRefl2SpectCyan.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectCyan, nRGB2SpectSamples, w0, w1);
            rgbRefl2SpectMagenta.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectMagenta, nRGB2SpectSamples, w0, w1);
            rgbRefl2SpectYellow.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectYellow, nRGB2SpectSamples, w0, w1);
            rgbRefl2SpectRed.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectRed, nRGB2SpectSamples, w0, w1);
            rgbRefl2SpectGreen.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectGreen, nRGB2SpectSamples, w0, w1);
            rgbRefl2SpectBlue.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectBlue, nRGB2SpectSamples, w0, w1);

            rgbIllum2SpectWhite.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectWhite, nRGB2SpectSamples, w0, w1);
            rgbIllum2SpectCyan.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectCyan, nRGB2SpectSamples, w0, w1);
            rgbIllum2SpectMagenta.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectMagenta, nRGB2SpectSamples, w0, w1);
            rgbIllum2SpectYellow.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectYellow, nRGB2SpectSamples, w0, w1);
            rgbIllum2SpectRed.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectRed, nRGB2SpectSamples, w0, w1);
            rgbIllum2SpectGreen.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectGreen, nRGB2SpectSamples, w0, w1);
            rgbIllum2SpectBlue.spectrumValues[i] = AverageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectBlue, nRGB2SpectSamples, w0, w1);
        }
    }

    static StaticArray<float, 3> SpectrumtoRGB(const Spectrum<float> &s)
    {
        StaticArray<float, 3> xyz, rgb;
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            xyz.params[0] += CIE_X_Sampled[i] * s.spectrumValues[i];
            xyz.params[1] += CIE_Y_Sampled[i] * s.spectrumValues[i];
            xyz.params[2] += CIE_Z_Sampled[i] * s.spectrumValues[i];
        }

        float xyzSum = float(WavelengthEnd - WavelengthStart) / float(CIE_Y_Integral * WavelengthSamplesSize);
        xyz *= xyzSum;

        rgb.params[0] = 3.240479f * xyz.params[0] - 1.537150f * xyz.params[1] - 0.498535f * xyz.params[2];
        rgb.params[1] = -0.969256f * xyz.params[0] + 1.875991f * xyz.params[1] + 0.041556f * xyz.params[2];
        rgb.params[2] = 0.055648f * xyz.params[0] - 0.204043f * xyz.params[1] + 1.057311f * xyz.params[2];

        return rgb;
    }

} // namespace filianore

#endif