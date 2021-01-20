#ifndef _SPECTRUM_H
#define _SPECTRUM_H

#include "spectrumfoundation.h"

namespace filianore
{
    enum class SpectrumType
    {
        REFLECTANCE,
        ILLUMINANT
    };

    template <typename T>
    class Spectrum
    {
    public:
        T spectrumValues[WavelengthSamplesSize];

        Spectrum()
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] = 0.f;
            }
        }

        Spectrum(T v)
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

        T Luminance()
        {
            T luminance = 0;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                luminance += CIE_Y[i] * spectrumValues[i];
            }
            return luminance * T(WavelengthEnd - WavelengthStart) / T(CIE_Y_Integral * WavelengthSamplesSize);
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

        T ValueAtWavelength(const T wavelength) const
        {
            for (int w = 0; w < WavelengthSamplesSize; w++)
            {
                if ((int)wavelength == wavelengthAvgs[w])
                {
                    return spectrumValues[w];
                }
            }
        }

        T ValueAtWavelengthIndex(const int wavelengthIdx)
        {
            return spectrumValues[wavelengthIdx];
        }

        T MaxSpectralValue() const
        {
            T mx = spectrumValues[0];
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

        Spectrum operator*(T s) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                clr1.spectrumValues[i] *= s;
            }
            return clr1;
        }

        Spectrum operator*=(T s)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                spectrumValues[i] *= s;
            }
            return *this;
        }

        Spectrum operator/(T s) const
        {
            Spectrum clr1 = *this;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                assert(s != 0);
                clr1.spectrumValues[i] /= s;
            }
            return clr1;
        }

        Spectrum operator/=(T s)
        {
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                assert(s != 0);
                spectrumValues[i] /= s;
            }
            return *this;
        }

        Spectrum SpecClamp(T low = 0.f, T high = Infinity<T>())
        {
            Spectrum result;
            for (int i = 0; i < WavelengthSamplesSize; ++i)
            {
                result.spectrumValues[i] = Clamp<T>(spectrumValues[i], low, high);
            }
            return result;
        }
    };

    template <typename T>
    static Spectrum<T> SpecScalarMult(const Spectrum<T> &clr, T s)
    {
        return clr * s;
    }

    template <typename T>
    static Spectrum<T> SpecScalarDiv(const Spectrum<T> &clr, T s)
    {
        return clr / s;
    }

    template <typename T>
    static Spectrum<T> SpecSqrt(const Spectrum<T> &clr)
    {
        Spectrum<T> result;
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            result.spectrumValues[i] = std::sqrt(clr.spectrumValues[i]);
        }
        return result;
    }

    template <typename T>
    static Spectrum<T> SpecPow(const Spectrum<T> &clr, const T exp)
    {
        Spectrum<T> result;
        for (int i = 0; i < WavelengthSamplesSize; ++i)
        {
            result.spectrumValues[i] = std::powf(clr.spectrumValues[i], exp);
        }
        return result;
    }

} // namespace filianore

#endif