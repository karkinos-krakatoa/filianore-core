#ifndef _SPECTRUM_H
#define _SPECTRUM_H

#include "spectrumfoundation.h"
#include <assert.h>

namespace filianore
{
    enum class SpectrumType
    {
        REFLECTANCE,
        ILLUMINANT
    };

    class Spectrum
    {
    public:
        float spectrumValues[WavelengthSamplesSize];

        Spectrum()
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] = 0.f;
            }
        }

        Spectrum(float v)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] = v;
            }
        }

        Spectrum(const Spectrum &clr)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] = clr.spectrumValues[i];
            }
        }

        bool IsBlack() const
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                if (spectrumValues[i] != 0.)
                {
                    return false;
                }
            }
            return true;
        }

        float Luminance()
        {
            float luminance = 0;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                luminance += CIE_Y[i] * spectrumValues[i];
            }
            return luminance * float(WavelengthEnd - WavelengthStart) / float(CIE_Y_Integral * WavelengthSamplesSize);
        }

        Spectrum &operator=(const Spectrum &clr)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] = clr.spectrumValues[i];
            }
            return *this;
        }

        bool operator==(const Spectrum &clr) const
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                if (spectrumValues[i] != clr.spectrumValues[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Spectrum &clr) const
        {
            return !(*this == clr);
        }

        float ValueAtWavelength(const float wavelength) const
        {
            for (int w = 0; w < WavelengthSamplesSize; w++)
            {
                if ((int)wavelength == wavelengthAvgs[w])
                {
                    return spectrumValues[w];
                }
            }
            return 0.f;
        }

        float ValueAtWavelengthIndex(const int wavelengthIdx)
        {
            return spectrumValues[wavelengthIdx];
        }

        float MaxSpectralValue() const
        {
            float mx = spectrumValues[0];
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                mx = std::max(mx, spectrumValues[i]);
            }
            return mx;
        }

        Spectrum operator+(const Spectrum &clr2) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                clr1.spectrumValues[i] += clr2.spectrumValues[i];
            }
            return clr1;
        }

        Spectrum operator+=(const Spectrum &clr2)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] += clr2.spectrumValues[i];
            }
            return *this;
        }

        Spectrum operator-(const Spectrum &clr2) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                clr1.spectrumValues[i] -= clr2.spectrumValues[i];
            }
            return clr1;
        }

        Spectrum operator-=(const Spectrum &clr2)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] -= clr2.spectrumValues[i];
            }
            return *this;
        }

        Spectrum operator/(const Spectrum &clr2) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                assert(clr2.spectrumValues[i] != 0);
                clr1.spectrumValues[i] /= clr2.spectrumValues[i];
            }
            return clr1;
        }

        Spectrum operator/=(const Spectrum &clr2)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                assert(clr2.spectrumValues[i] != 0);
                spectrumValues[i] /= clr2.spectrumValues[i];
            }
            return *this;
        }

        Spectrum operator*(const Spectrum &clr2) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                clr1.spectrumValues[i] *= clr2.spectrumValues[i];
            }
            return clr1;
        }

        Spectrum operator*=(const Spectrum &clr2)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] *= clr2.spectrumValues[i];
            }
            return *this;
        }

        Spectrum operator*(float s) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                clr1.spectrumValues[i] *= s;
            }
            return clr1;
        }

        Spectrum operator*=(float s)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] *= s;
            }
            return *this;
        }

        Spectrum operator/(float s) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                assert(s != 0);
                clr1.spectrumValues[i] /= s;
            }
            return clr1;
        }

        Spectrum operator/=(float s)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                assert(s != 0);
                spectrumValues[i] /= s;
            }
            return *this;
        }

        Spectrum SpecClamp(float low = 0.f, float high = Infinity<float>())
        {
            Spectrum result;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                result.spectrumValues[i] = Clamp<float>(spectrumValues[i], low, high);
            }
            return result;
        }
    };

    static Spectrum SpecScalarMult(const Spectrum &clr, float s)
    {
        return clr * s;
    }

    static Spectrum SpecScalarDiv(const Spectrum &clr, float s)
    {
        return clr / s;
    }

    static Spectrum SpecSqrt(const Spectrum &clr)
    {
        Spectrum result;
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            result.spectrumValues[i] = std::sqrt(clr.spectrumValues[i]);
        }
        return result;
    }

    static Spectrum SpecPow(const Spectrum &clr, float exp)
    {
        Spectrum result;
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            result.spectrumValues[i] = std::pow(clr.spectrumValues[i], exp);
        }
        return result;
    }

} // namespace filianore

#endif