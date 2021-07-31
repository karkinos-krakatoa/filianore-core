#include "filianore/color/spectrumoperations.h"
#include "filianore/color/spectruminits.h"
#include "filianore/color/cie.h"

namespace filianore
{

    bool SpectrumSamplesSorted(const float *wavelengths, const float *values, int n)
    {
        for (int i = 0; i < n - 1; ++i)
            if (wavelengths[i] > wavelengths[i + 1])
                return false;
        return true;
    }

    void SortSpectrumSamples(float *wavelengths, float *values, int n)
    {
        std::vector<std::pair<float, float>> sortVec;
        sortVec.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            sortVec.push_back(std::make_pair(wavelengths[i], values[i]));
        }
        std::sort(sortVec.begin(), sortVec.end());
        for (int i = 0; i < n; ++i)
        {
            wavelengths[i] = sortVec[i].first;
            values[i] = sortVec[i].second;
        }
    }

    float AverageSpectrumSamples(const float *wavelength, const float *vals, int n, float wavelengthStart, float wavelengthEnd)
    {
        if (wavelengthEnd <= wavelength[0])
            return vals[0];
        if (wavelengthStart >= wavelength[n - 1])
            return vals[n - 1];
        if (n == 1)
            return vals[0];

        float sum = 0;
        if (wavelengthStart < wavelength[0])
            sum += vals[0] * (wavelength[0] - wavelengthStart);
        if (wavelengthEnd > wavelength[n - 1])
            sum += vals[n - 1] * (wavelengthEnd - wavelength[n - 1]);

        int i = 0;
        while (wavelengthStart > wavelength[i + 1])
            ++i;

        auto interp = [wavelength, vals](float w, int i)
        {
            return Lerp<float>((w - wavelength[i]) / (wavelength[i + 1] - wavelength[i]), vals[i], vals[i + 1]);
        };

        for (; i + 1 < n && wavelengthEnd >= wavelength[i]; ++i)
        {
            float segwavelengthStart = std::max(wavelengthStart, wavelength[i]);
            float segwavelengthEnd = std::min(wavelengthEnd, wavelength[i + 1]);
            sum += 0.5 * (interp(segwavelengthStart, i) + interp(segwavelengthEnd, i)) * (segwavelengthEnd - segwavelengthStart);
        }
        return sum / (wavelengthEnd - wavelengthStart);
    }

    StaticArray<float, 3> XYZToRGB(const StaticArray<float, 3> &xyz)
    {
        StaticArray<float, 3> rgb(0.f);

        rgb.params[0] = 3.240479f * xyz.params[0] - 1.537150f * xyz.params[1] - 0.498535f * xyz.params[2];
        rgb.params[1] = -0.969256f * xyz.params[0] + 1.875991f * xyz.params[1] + 0.041556f * xyz.params[2];
        rgb.params[2] = 0.055648f * xyz.params[0] - 0.204043f * xyz.params[1] + 1.057311f * xyz.params[2];

        return rgb;
    }

    StaticArray<float, 3> XYZToACES2065_1(const StaticArray<float, 3> &xyz)
    {
        StaticArray<float, 3> aces(0.f);

        aces.params[0] = 1.0498110175f * xyz.params[0] + 0.0000000000f * xyz.params[1] - 0.0000974845f * xyz.params[2];
        aces.params[1] = -0.4959030231f * xyz.params[0] + 1.3733130458f * xyz.params[1] + 0.0982400361f * xyz.params[2];
        aces.params[2] = 0.0000000000f * xyz.params[0] + 0.0000000000f * xyz.params[1] + 0.9912520182f * xyz.params[2];

        return aces;
    }

    StaticArray<float, 3> RGBToXYZ(const StaticArray<float, 3> &rgb)
    {
        StaticArray<float, 3> xyz(0.f);

        xyz.params[0] = 0.412453f * rgb.params[0] + 0.357580f * rgb.params[1] + 0.180423f * rgb.params[2];
        xyz.params[1] = 0.212671f * rgb.params[0] + 0.715160f * rgb.params[1] + 0.072169f * rgb.params[2];
        xyz.params[2] = 0.019334f * rgb.params[0] + 0.119193f * rgb.params[1] + 0.950227f * rgb.params[2];

        return xyz;
    }

    PrincipalSpectrum FromSPD(const float *wavelengths, const float *values, int n)
    {
        if (!SpectrumSamplesSorted(wavelengths, values, n))
        {
            std::vector<float> swavelength(&wavelengths[0], &wavelengths[n]);
            std::vector<float> sv(&values[0], &values[n]);
            SortSpectrumSamples(&swavelength[0], &sv[0], n);
            return FromSPD(&swavelength[0], &sv[0], n);
        }

        PrincipalSpectrum r;
        for (int i = 0; i < nSpectralSamples; ++i)
        {
            float wavelength0 = Lerp<float>(float(i) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
            float wavelength1 = Lerp<float>(float(i + 1) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
            r.c[i] = AverageSpectrumSamples(wavelengths, values, n, wavelength0, wavelength1);
        }
        return r;
    }

    PrincipalSpectrum FromSPDExact(const float *values)
    {
        PrincipalSpectrum r;
        for (int i = 0; i < nSpectralSamples; ++i)
        {
            r.c[i] = values[i];
        }
        return r;
    }

    PrincipalSpectrum FromReflectanceRGB(const StaticArray<float, 3> &rgb)
    {
        PrincipalSpectrum r(0.f);

        if (rgb.params[0] <= rgb.params[1] && rgb.params[0] <= rgb.params[2])
        {
            r += rgb.params[0] * rgbRefl2SpectWhite;
            if (rgb.params[1] <= rgb.params[2])
            {
                r += (rgb.params[1] - rgb.params[0]) * rgbRefl2SpectCyan;
                r += (rgb.params[2] - rgb.params[1]) * rgbRefl2SpectBlue;
            }
            else
            {
                r += (rgb.params[2] - rgb.params[0]) * rgbRefl2SpectCyan;
                r += (rgb.params[1] - rgb.params[2]) * rgbRefl2SpectGreen;
            }
        }
        else if (rgb.params[1] <= rgb.params[0] && rgb.params[1] <= rgb.params[2])
        {
            r += rgb.params[1] * rgbRefl2SpectWhite;
            if (rgb.params[0] <= rgb.params[2])
            {
                r += (rgb.params[0] - rgb.params[1]) * rgbRefl2SpectMagenta;
                r += (rgb.params[2] - rgb.params[0]) * rgbRefl2SpectBlue;
            }
            else
            {
                r += (rgb.params[2] - rgb.params[1]) * rgbRefl2SpectMagenta;
                r += (rgb.params[0] - rgb.params[2]) * rgbRefl2SpectRed;
            }
        }
        else
        {
            r += rgb.params[2] * rgbRefl2SpectWhite;
            if (rgb.params[0] <= rgb.params[1])
            {
                r += (rgb.params[0] - rgb.params[2]) * rgbRefl2SpectYellow;
                r += (rgb.params[1] - rgb.params[0]) * rgbRefl2SpectGreen;
            }
            else
            {
                r += (rgb.params[1] - rgb.params[2]) * rgbRefl2SpectYellow;
                r += (rgb.params[0] - rgb.params[1]) * rgbRefl2SpectRed;
            }
        }
        r *= .94;

        return r.SpectrumClamp();
    }

    PrincipalSpectrum FromIlluminanceRGB(const StaticArray<float, 3> &rgb)
    {
        PrincipalSpectrum r(0.f);

        if (rgb.params[0] <= rgb.params[1] && rgb.params[0] <= rgb.params[2])
        {
            r += rgb.params[0] * rgbIllum2SpectWhite;
            if (rgb.params[1] <= rgb.params[2])
            {
                r += (rgb.params[1] - rgb.params[0]) * rgbIllum2SpectCyan;
                r += (rgb.params[2] - rgb.params[1]) * rgbIllum2SpectBlue;
            }
            else
            {
                r += (rgb.params[2] - rgb.params[0]) * rgbIllum2SpectCyan;
                r += (rgb.params[1] - rgb.params[2]) * rgbIllum2SpectGreen;
            }
        }
        else if (rgb.params[1] <= rgb.params[0] && rgb.params[1] <= rgb.params[2])
        {
            r += rgb.params[1] * rgbIllum2SpectWhite;
            if (rgb.params[0] <= rgb.params[2])
            {
                r += (rgb.params[0] - rgb.params[1]) * rgbIllum2SpectMagenta;
                r += (rgb.params[2] - rgb.params[0]) * rgbIllum2SpectBlue;
            }
            else
            {
                r += (rgb.params[2] - rgb.params[1]) * rgbIllum2SpectMagenta;
                r += (rgb.params[0] - rgb.params[2]) * rgbIllum2SpectRed;
            }
        }
        else
        {
            r += rgb.params[2] * rgbIllum2SpectWhite;
            if (rgb.params[0] <= rgb.params[1])
            {
                r += (rgb.params[0] - rgb.params[2]) * rgbIllum2SpectYellow;
                r += (rgb.params[1] - rgb.params[0]) * rgbIllum2SpectGreen;
            }
            else
            {
                r += (rgb.params[1] - rgb.params[2]) * rgbIllum2SpectYellow;
                r += (rgb.params[0] - rgb.params[1]) * rgbIllum2SpectRed;
            }
        }
        r *= .86445f;

        return r.SpectrumClamp();
    }

    float Luminance(const PrincipalSpectrum &r)
    {
        float yy = 0.f;
        for (int i = 0; i < nSpectralSamples; ++i)
        {
            yy += Y.c[i] * r.c[i];
        }
        return yy * float(sampledWavelengthEnd - sampledWavelengthStart) / float(nSpectralSamples);
    }

    StaticArray<float, 3> ToXYZ(const PrincipalSpectrum &r)
    {
        StaticArray<float, 3> xyz(0.f);

        for (int i = 0; i < nSpectralSamples; ++i)
        {
            xyz.params[0] += X.c[i] * r.c[i];
            xyz.params[1] += Y.c[i] * r.c[i];
            xyz.params[2] += Z.c[i] * r.c[i];
        }

        float scale = float(sampledWavelengthEnd - sampledWavelengthStart) / float(CIE_Y_Integral * nSpectralSamples);

        xyz *= scale;

        return xyz;
    }

    StaticArray<float, 3> ToRGB(const PrincipalSpectrum &r)
    {
        StaticArray<float, 3> xyz = ToXYZ(r);
        return XYZToRGB(xyz);
    }

    StaticArray<float, 3> GammaCorrect(StaticArray<float, 3> &inputColor, int gammaCorrectionType)
    {
        if (gammaCorrectionType == GammaCorrectionType::NONE)
        {
            return inputColor;
        }
        else if (gammaCorrectionType == GammaCorrectionType::REC_709)
        {
            if (inputColor.params[0] >= 0.018f)
                inputColor.params[0] = (1.099f * powf(inputColor.params[0], 0.45f)) - 0.099f;
            else
                inputColor.params[0] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
            if (inputColor.params[1] >= 0.018f)
                inputColor.params[1] = (1.099f * powf(inputColor.params[1], 0.45f)) - 0.099f;
            else
                inputColor.params[1] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
            if (inputColor.params[2] >= 0.018f)
                inputColor.params[2] = (1.099f * powf(inputColor.params[2], 0.45f)) - 0.099f;
            else
                inputColor.params[2] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;

            return inputColor;
        }
        else if (gammaCorrectionType == GammaCorrectionType::GAMMA_1_8)
        {
            float power = 1.f / 1.8f;
            return StaticArray<float, 3>(powf(inputColor.params[0], power), powf(inputColor.params[1], power), powf(inputColor.params[2], power));
        }
        else if (gammaCorrectionType == GammaCorrectionType::GAMMA_2_2)
        {
            float power = 1.f / 2.2f;
            return StaticArray<float, 3>(powf(inputColor.params[0], power), powf(inputColor.params[1], power), powf(inputColor.params[2], power));
        }
        else if (gammaCorrectionType == GammaCorrectionType::GAMMA_4_0)
        {
            float power = 1.f / 4.f;
            return StaticArray<float, 3>(powf(inputColor.params[0], power), powf(inputColor.params[1], power), powf(inputColor.params[2], power));
        }

        return inputColor;
    }

    StaticArray<float, 3> ToneMap(StaticArray<float, 3> &inputColor, int tonemapType)
    {
        if (tonemapType == ToneMappingType::NONE)
        {
            return inputColor;
        }
        else if (tonemapType == ToneMappingType::ACES_BASIC)
        {
            const float a = 2.51f;
            const float b = 0.03f;
            const float c = 2.43f;
            const float d = 0.59f;
            const float e = 0.14f;

            float rr = Clamp<float>((inputColor.params[0] * (a * inputColor.params[0] + b)) / (inputColor.params[0] * (c * inputColor.params[0] + d) + e), 0.f, 1.f);
            float gg = Clamp<float>((inputColor.params[1] * (a * inputColor.params[1] + b)) / (inputColor.params[1] * (c * inputColor.params[1] + d) + e), 0.f, 1.f);
            float bb = Clamp<float>((inputColor.params[2] * (a * inputColor.params[2] + b)) / (inputColor.params[2] * (c * inputColor.params[2] + d) + e), 0.f, 1.f);

            return StaticArray<float, 3>(rr, gg, bb);
        }
        else if (tonemapType == ToneMappingType::FILMIC)
        {
            float X = std::max(0.f, inputColor.params[0] - 0.004f);
            float result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
            float rr = pow(result, 2.2f);

            X = std::max(0.f, inputColor.params[1] - 0.004f);
            result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
            float gg = pow(result, 2.2f);

            X = std::max(0.f, inputColor.params[2] - 0.004f);
            result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
            float bb = pow(result, 2.2f);

            return StaticArray<float, 3>(rr, gg, bb);
        }
        else if (tonemapType == ToneMappingType::REINHARD)
        {
            const float L_white = 4.f;

            float rr = (inputColor.params[0] * (1.f + inputColor.params[0] / (L_white * L_white))) / (1.f + inputColor.params[0]);
            float gg = (inputColor.params[1] * (1.f + inputColor.params[1] / (L_white * L_white))) / (1.f + inputColor.params[1]);
            float bb = (inputColor.params[2] * (1.f + inputColor.params[2] / (L_white * L_white))) / (1.f + inputColor.params[2]);

            return StaticArray<float, 3>(rr, gg, bb);
        }

        return inputColor;
    }

}