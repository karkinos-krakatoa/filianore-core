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

        auto interp = [wavelength, vals](float w, int i) {
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

    StaticArray<float, 3> GammaCorrect(StaticArray<float, 3> &rgb)
    {
        if (rgb.params[0] >= 0.018f)
            rgb.params[0] = (1.099f * powf(rgb.params[0], 0.45f)) - 0.099f;
        else
            rgb.params[0] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
        if (rgb.params[1] >= 0.018f)
            rgb.params[1] = (1.099f * powf(rgb.params[1], 0.45f)) - 0.099f;
        else
            rgb.params[1] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
        if (rgb.params[2] >= 0.018f)
            rgb.params[2] = (1.099f * powf(rgb.params[2], 0.45f)) - 0.099f;
        else
            rgb.params[2] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;

        // Return the gamma-corrected color
        return rgb;
    }
}