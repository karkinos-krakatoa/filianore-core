#include "spectrumoperations.h"
#include "../maths/mathutils.h"
#include "../maths/vec.h"
#include "cie.h"
#include "spectruminits.h"

namespace filianore {

bool spectrum_samples_sorted(const float *wavelengths, const float *values, int n) {
    for (int i = 0; i < n - 1; ++i)
        if (wavelengths[i] > wavelengths[i + 1])
            return false;
    return true;
}

void sort_spectrum_samples(float *wavelengths, float *values, int n) {
    std::vector<std::pair<float, float>> sortVec;
    sortVec.reserve(n);
    for (int i = 0; i < n; ++i) {
        sortVec.push_back(std::make_pair(wavelengths[i], values[i]));
    }
    std::sort(sortVec.begin(), sortVec.end());
    for (int i = 0; i < n; ++i) {
        wavelengths[i] = sortVec[i].first;
        values[i] = sortVec[i].second;
    }
}

float average_spectrum_samples(const float *wavelength, const float *vals, int n, float wavelengthStart, float wavelengthEnd) {
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
        return lerp<float>((w - wavelength[i]) / (wavelength[i + 1] - wavelength[i]), vals[i], vals[i + 1]);
    };

    for (; i + 1 < n && wavelengthEnd >= wavelength[i]; ++i) {
        float segwavelengthStart = std::max(wavelengthStart, wavelength[i]);
        float segwavelengthEnd = std::min(wavelengthEnd, wavelength[i + 1]);
        sum += 0.5 * (interp(segwavelengthStart, i) + interp(segwavelengthEnd, i)) * (segwavelengthEnd - segwavelengthStart);
    }
    return sum / (wavelengthEnd - wavelengthStart);
}

Vector3f xyz_to_rgb(const Vector3f &xyz) {
    Vector3f rgb(0.f);

    rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
    rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
    rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];

    return rgb;
}

Vector3f xyz_to_aces2065_1(const Vector3f &xyz) {
    Vector3f aces(0.f);

    aces[0] = 1.0498110175f * xyz[0] + 0.0000000000f * xyz[1] - 0.0000974845f * xyz[2];
    aces[1] = -0.4959030231f * xyz[0] + 1.3733130458f * xyz[1] + 0.0982400361f * xyz[2];
    aces[2] = 0.0000000000f * xyz[0] + 0.0000000000f * xyz[1] + 0.9912520182f * xyz[2];

    return aces;
}

Vector3f RGBToXYZ(const Vector3f &rgb) {
    Vector3f xyz(0.f);

    xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
    xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
    xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];

    return xyz;
}

PrincipalSpectrum from_spd(const float *wavelengths, const float *values, int n) {
    if (!spectrum_samples_sorted(wavelengths, values, n)) {
        std::vector<float> swavelength(&wavelengths[0], &wavelengths[n]);
        std::vector<float> sv(&values[0], &values[n]);
        sort_spectrum_samples(&swavelength[0], &sv[0], n);
        return from_spd(&swavelength[0], &sv[0], n);
    }

    PrincipalSpectrum r;
    for (int i = 0; i < nSpectralSamples; ++i) {
        float wavelength0 = lerp<float>(float(i) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
        float wavelength1 = lerp<float>(float(i + 1) / float(nSpectralSamples), sampledWavelengthStart, sampledWavelengthEnd);
        r.c[i] = average_spectrum_samples(wavelengths, values, n, wavelength0, wavelength1);
    }
    return r;
}

PrincipalSpectrum from_spd_exact(const float *values) {
    PrincipalSpectrum r;
    for (int i = 0; i < nSpectralSamples; ++i) {
        r.c[i] = values[i];
    }
    return r;
}

PrincipalSpectrum from_reflectance_rgb(const Vector3f &rgb) {
    PrincipalSpectrum r(0.f);

    if (rgb[0] <= rgb[1] && rgb[0] <= rgb[2]) {
        r += rgb[0] * rgbRefl2SpectWhite;
        if (rgb[1] <= rgb[2]) {
            r += (rgb[1] - rgb[0]) * rgbRefl2SpectCyan;
            r += (rgb[2] - rgb[1]) * rgbRefl2SpectBlue;
        } else {
            r += (rgb[2] - rgb[0]) * rgbRefl2SpectCyan;
            r += (rgb[1] - rgb[2]) * rgbRefl2SpectGreen;
        }
    } else if (rgb[1] <= rgb[0] && rgb[1] <= rgb[2]) {
        r += rgb[1] * rgbRefl2SpectWhite;
        if (rgb[0] <= rgb[2]) {
            r += (rgb[0] - rgb[1]) * rgbRefl2SpectMagenta;
            r += (rgb[2] - rgb[0]) * rgbRefl2SpectBlue;
        } else {
            r += (rgb[2] - rgb[1]) * rgbRefl2SpectMagenta;
            r += (rgb[0] - rgb[2]) * rgbRefl2SpectRed;
        }
    } else {
        r += rgb[2] * rgbRefl2SpectWhite;
        if (rgb[0] <= rgb[1]) {
            r += (rgb[0] - rgb[2]) * rgbRefl2SpectYellow;
            r += (rgb[1] - rgb[0]) * rgbRefl2SpectGreen;
        } else {
            r += (rgb[1] - rgb[2]) * rgbRefl2SpectYellow;
            r += (rgb[0] - rgb[1]) * rgbRefl2SpectRed;
        }
    }
    r *= .94;

    return r.spectrum_clamp();
}

PrincipalSpectrum from_illuminance_rgb(const Vector3f &rgb) {
    PrincipalSpectrum r(0.f);

    if (rgb[0] <= rgb[1] && rgb[0] <= rgb[2]) {
        r += rgb[0] * rgbIllum2SpectWhite;
        if (rgb[1] <= rgb[2]) {
            r += (rgb[1] - rgb[0]) * rgbIllum2SpectCyan;
            r += (rgb[2] - rgb[1]) * rgbIllum2SpectBlue;
        } else {
            r += (rgb[2] - rgb[0]) * rgbIllum2SpectCyan;
            r += (rgb[1] - rgb[2]) * rgbIllum2SpectGreen;
        }
    } else if (rgb[1] <= rgb[0] && rgb[1] <= rgb[2]) {
        r += rgb[1] * rgbIllum2SpectWhite;
        if (rgb[0] <= rgb[2]) {
            r += (rgb[0] - rgb[1]) * rgbIllum2SpectMagenta;
            r += (rgb[2] - rgb[0]) * rgbIllum2SpectBlue;
        } else {
            r += (rgb[2] - rgb[1]) * rgbIllum2SpectMagenta;
            r += (rgb[0] - rgb[2]) * rgbIllum2SpectRed;
        }
    } else {
        r += rgb[2] * rgbIllum2SpectWhite;
        if (rgb[0] <= rgb[1]) {
            r += (rgb[0] - rgb[2]) * rgbIllum2SpectYellow;
            r += (rgb[1] - rgb[0]) * rgbIllum2SpectGreen;
        } else {
            r += (rgb[1] - rgb[2]) * rgbIllum2SpectYellow;
            r += (rgb[0] - rgb[1]) * rgbIllum2SpectRed;
        }
    }
    r *= .86445f;

    return r.spectrum_clamp();
}

float luminance(const PrincipalSpectrum &r) {
    float yy = 0.f;
    for (int i = 0; i < nSpectralSamples; ++i) {
        yy += Y.c[i] * r.c[i];
    }
    return yy * float(sampledWavelengthEnd - sampledWavelengthStart) / float(nSpectralSamples);
}

Vector3f to_xyz(const PrincipalSpectrum &r) {
    Vector3f xyz(0.f);

    for (int i = 0; i < nSpectralSamples; ++i) {
        xyz[0] += X.c[i] * r.c[i];
        xyz[1] += Y.c[i] * r.c[i];
        xyz[2] += Z.c[i] * r.c[i];
    }

    float scale = float(sampledWavelengthEnd - sampledWavelengthStart) / float(CIE_Y_Integral * nSpectralSamples);

    xyz *= scale;

    return xyz;
}

Vector3f to_rgb(const PrincipalSpectrum &r) {
    Vector3f xyz = to_xyz(r);
    return xyz_to_rgb(xyz);
}

Vector3f gamma_correct(Vector3f &inputColor, int gammaCorrectionType) {
    if (gammaCorrectionType == GammaCorrectionType::NONE) {
        return inputColor;
    } else if (gammaCorrectionType == GammaCorrectionType::REC_709) {
        if (inputColor[0] >= 0.018f)
            inputColor[0] = (1.099f * powf(inputColor[0], 0.45f)) - 0.099f;
        else
            inputColor[0] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
        if (inputColor[1] >= 0.018f)
            inputColor[1] = (1.099f * powf(inputColor[1], 0.45f)) - 0.099f;
        else
            inputColor[1] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
        if (inputColor[2] >= 0.018f)
            inputColor[2] = (1.099f * powf(inputColor[2], 0.45f)) - 0.099f;
        else
            inputColor[2] *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;

        return inputColor;
    } else if (gammaCorrectionType == GammaCorrectionType::GAMMA_1_8) {
        float power = 1.f / 1.8f;
        return Vector3f(powf(inputColor[0], power), powf(inputColor[1], power), powf(inputColor[2], power));
    } else if (gammaCorrectionType == GammaCorrectionType::GAMMA_2_2) {
        float power = 1.f / 2.2f;
        return Vector3f(powf(inputColor[0], power), powf(inputColor[1], power), powf(inputColor[2], power));
    } else if (gammaCorrectionType == GammaCorrectionType::GAMMA_4_0) {
        float power = 1.f / 4.f;
        return Vector3f(powf(inputColor[0], power), powf(inputColor[1], power), powf(inputColor[2], power));
    }

    return inputColor;
}

Vector3f tone_map(Vector3f &inputColor, int tonemapType) {
    if (tonemapType == ToneMappingType::NONE) {
        return inputColor;
    } else if (tonemapType == ToneMappingType::ACES_BASIC) {
        const float a = 2.51f;
        const float b = 0.03f;
        const float c = 2.43f;
        const float d = 0.59f;
        const float e = 0.14f;

        float rr = filianore::clamp((inputColor[0] * (a * inputColor[0] + b)) / (inputColor[0] * (c * inputColor[0] + d) + e), 0.f, 1.f);
        float gg = filianore::clamp((inputColor[1] * (a * inputColor[1] + b)) / (inputColor[1] * (c * inputColor[1] + d) + e), 0.f, 1.f);
        float bb = filianore::clamp((inputColor[2] * (a * inputColor[2] + b)) / (inputColor[2] * (c * inputColor[2] + d) + e), 0.f, 1.f);

        return Vector3f(rr, gg, bb);
    } else if (tonemapType == ToneMappingType::FILMIC) {
        float X = std::max(0.f, inputColor[0] - 0.004f);
        float result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
        float rr = powf(result, 2.2f);

        X = std::max(0.f, inputColor[1] - 0.004f);
        result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
        float gg = powf(result, 2.2f);

        X = std::max(0.f, inputColor[2] - 0.004f);
        result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
        float bb = powf(result, 2.2f);

        return Vector3f(rr, gg, bb);
    } else if (tonemapType == ToneMappingType::REINHARD) {
        const float L_white = 4.f;

        float rr = (inputColor[0] * (1.f + inputColor[0] / (L_white * L_white))) / (1.f + inputColor[0]);
        float gg = (inputColor[1] * (1.f + inputColor[1] / (L_white * L_white))) / (1.f + inputColor[1]);
        float bb = (inputColor[2] * (1.f + inputColor[2] / (L_white * L_white))) / (1.f + inputColor[2]);

        return Vector3f(rr, gg, bb);
    }

    return inputColor;
}

} // namespace filianore