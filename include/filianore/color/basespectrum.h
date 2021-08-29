#ifndef _BASE_SPECTRUM_H
#define _BASE_SPECTRUM_H

#include <vector>
#include "../maths/scalar.h"

namespace filianore
{

    template <int nSpectrumSamples>
    class BaseSpectrum
    {
    public:
        BaseSpectrum(float v = 0.f)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] = v;
            }
        }

        BaseSpectrum(const std::vector<float> &v)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] = v[i];
            }
        }

        BaseSpectrum(const BaseSpectrum &s)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] = s.c[i];
            }
        }

        BaseSpectrum &operator=(const BaseSpectrum &s)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] = s.c[i];
            }
            return *this;
        }

        BaseSpectrum &operator+=(const BaseSpectrum &s2)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] += s2.c[i];
            }
            return *this;
        }

        BaseSpectrum operator+(const BaseSpectrum &s2) const
        {
            BaseSpectrum ret = *this;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] += s2.c[i];
            }
            return ret;
        }

        BaseSpectrum operator-(const BaseSpectrum &s2) const
        {
            BaseSpectrum ret = *this;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] -= s2.c[i];
            }
            return ret;
        }

        BaseSpectrum operator/(const BaseSpectrum &s2) const
        {
            BaseSpectrum ret = *this;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] /= s2.c[i];
            }
            return ret;
        }

        BaseSpectrum operator*(const BaseSpectrum &sp) const
        {
            BaseSpectrum ret = *this;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] *= sp.c[i];
            }
            return ret;
        }

        BaseSpectrum &operator*=(const BaseSpectrum &sp)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] *= sp.c[i];
            }
            return *this;
        }

        BaseSpectrum operator*(float a) const
        {
            BaseSpectrum ret = *this;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] *= a;
            }
            return ret;
        }

        BaseSpectrum &operator*=(float a)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] *= a;
            }
            return *this;
        }

        friend inline BaseSpectrum operator*(float a, const BaseSpectrum &s)
        {
            return s * a;
        }

        BaseSpectrum operator/(float a) const
        {
            BaseSpectrum ret = *this;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] /= a;
            }
            return ret;
        }

        BaseSpectrum &operator/=(float a)
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                c[i] /= a;
            }
            return *this;
        }

        bool operator==(const BaseSpectrum &sp) const
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
                if (c[i] != sp.c[i])
                    return false;
            return true;
        }

        bool operator!=(const BaseSpectrum &sp) const
        {
            return !(*this == sp);
        }

        bool IsBlack() const
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
                if (c[i] != 0.)
                    return false;
            return true;
        }

        friend BaseSpectrum Sqrt(const BaseSpectrum &s)
        {
            BaseSpectrum ret;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] = std::sqrt(s.c[i]);
            }
            return ret;
        }

        template <int n>
        friend inline BaseSpectrum<n> Pow(const BaseSpectrum<n> &s, float e);

        BaseSpectrum operator-() const
        {
            BaseSpectrum ret;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] = -c[i];
            }
            return ret;
        }

        friend BaseSpectrum Exp(const BaseSpectrum &s)
        {
            BaseSpectrum ret;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] = std::exp(s.c[i]);
            }
            return ret;
        }

        BaseSpectrum SpectrumClamp(float low = 0.f, float high = Infinity<float>()) const
        {
            BaseSpectrum ret;
            for (int i = 0; i < nSpectrumSamples; ++i)
            {
                ret.c[i] = Clamp<float>(c[i], low, high);
            }
            return ret;
        }

        float MaxComponentValue() const
        {
            float m = c[0];
            for (int i = 1; i < nSpectrumSamples; ++i)
            {
                m = std::max(m, c[i]);
            }
            return m;
        }

        bool HasNaNs() const
        {
            for (int i = 0; i < nSpectrumSamples; ++i)
                if (std::isnan(c[i]))
                    return true;
            return false;
        }

        float &operator[](int i)
        {
            return c[i];
        }

        float operator[](int i) const
        {
            return c[i];
        }

        static const int nSamples = nSpectrumSamples;

        float c[nSpectrumSamples];
    };

    template <int nSpectrumSamples>
    inline BaseSpectrum<nSpectrumSamples> Pow(const BaseSpectrum<nSpectrumSamples> &s, float e)
    {
        BaseSpectrum<nSpectrumSamples> ret;
        for (int i = 0; i < nSpectrumSamples; ++i)
        {
            ret.c[i] = std::pow(s.c[i], e);
        }
        return ret;
    }
}

#endif
