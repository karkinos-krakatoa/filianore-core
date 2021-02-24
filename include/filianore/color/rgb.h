#ifndef _RGB_H
#define _RGB_H

#include "../maths/scalar.h"
#include <assert.h>

namespace filianore
{

    // class RGBSpectrum
    // {
    // public:
    //     float r, g, b;

    //     RGBSpectrum()
    //         : r(0.f), g(0.f), b(0.f)
    //     {
    //     }

    //     RGBSpectrum(float val)
    //         : r(val), g(val), b(val)
    //     {
    //     }

    //     RGBSpectrum(float _r, float _g, float _b)
    //         : r(_r), g(_g), b(_b)
    //     {
    //     }

    //     RGBSpectrum(const RGBSpectrum &clr)
    //     {
    //         r = clr.r;
    //         g = clr.g;
    //         b = clr.b;
    //     }

    //     bool IsBlack() const
    //     {
    //         if (r == 0.f && g == 0.f && b == 0.f)
    //         {
    //             return true;
    //         }
    //         return false;
    //     }

    //     RGBSpectrum &operator=(const RGBSpectrum &clr)
    //     {
    //         r = clr.r;
    //         g = clr.g;
    //         b = clr.b;
    //         return *this;
    //     }

    //     bool operator==(const RGBSpectrum &clr) const
    //     {
    //         if (r == clr.r && g == clr.g && b == clr.b)
    //             return true;
    //         return false;
    //     }

    //     bool operator!=(const RGBSpectrum &clr) const
    //     {
    //         return !(*this == clr);
    //     }

    //     float MaxSpectralValue() const
    //     {
    //         float mx = r;

    //         mx = std::max(mx, g);
    //         mx = std::max(mx, b);

    //         return mx;
    //     }

    //     RGBSpectrum operator+(const RGBSpectrum &clr2) const
    //     {
    //         RGBSpectrum clr1 = *this;

    //         clr1.r += clr2.r;
    //         clr1.g += clr2.g;
    //         clr1.b += clr2.b;

    //         return clr1;
    //     }

    //     RGBSpectrum operator+=(const RGBSpectrum &clr2)
    //     {
    //         r += clr2.r;
    //         g += clr2.g;
    //         b += clr2.b;

    //         return *this;
    //     }

    //     RGBSpectrum operator-(const RGBSpectrum &clr2) const
    //     {
    //         RGBSpectrum clr1 = *this;

    //         clr1.r -= clr2.r;
    //         clr1.g -= clr2.g;
    //         clr1.b -= clr2.b;

    //         return clr1;
    //     }

    //     RGBSpectrum operator-=(const RGBSpectrum &clr2)
    //     {
    //         r -= clr2.r;
    //         g -= clr2.g;
    //         b -= clr2.b;

    //         return *this;
    //     }

    //     RGBSpectrum operator/(const RGBSpectrum &clr2) const
    //     {
    //         RGBSpectrum clr1 = *this;

    //         clr1.r /= clr2.r;
    //         clr1.g /= clr2.g;
    //         clr1.b /= clr2.b;

    //         return clr1;
    //     }

    //     RGBSpectrum operator/=(const RGBSpectrum &clr2)
    //     {
    //         r /= clr2.r;
    //         g /= clr2.g;
    //         b /= clr2.b;

    //         return *this;
    //     }

    //     RGBSpectrum operator*(const RGBSpectrum &clr2) const
    //     {
    //         RGBSpectrum clr1 = *this;

    //         clr1.r *= clr2.r;
    //         clr1.g *= clr2.g;
    //         clr1.b *= clr2.b;

    //         return clr1;
    //     }

    //     RGBSpectrum operator*=(const RGBSpectrum &clr2)
    //     {
    //         r *= clr2.r;
    //         g *= clr2.g;
    //         b *= clr2.b;

    //         return *this;
    //     }

    //     RGBSpectrum operator*(float s) const
    //     {
    //         RGBSpectrum clr1 = *this;

    //         clr1.r *= s;
    //         clr1.g *= s;
    //         clr1.b *= s;

    //         return clr1;
    //     }

    //     RGBSpectrum operator*=(float s)
    //     {
    //         r *= s;
    //         g *= s;
    //         b *= s;

    //         return *this;
    //     }

    //     RGBSpectrum operator/(float s) const
    //     {
    //         RGBSpectrum clr1 = *this;

    //         clr1.r /= s;
    //         clr1.g /= s;
    //         clr1.b /= s;

    //         return clr1;
    //     }

    //     RGBSpectrum operator/=(float s)
    //     {
    //         r /= s;
    //         g /= s;
    //         b /= s;

    //         return *this;
    //     }

    //     RGBSpectrum SpecClamp(float low = 0.f, float high = Infinity<float>())
    //     {
    //         RGBSpectrum result;

    //         result.r = Clamp<float>(r, low, high);
    //         result.g = Clamp<float>(g, low, high);
    //         result.b = Clamp<float>(b, low, high);

    //         return result;
    //     }
    // };

    // static RGBSpectrum SpecScalarMult(const RGBSpectrum &clr, float s)
    // {
    //     return clr * s;
    // }

    // static RGBSpectrum SpecScalarDiv(const RGBSpectrum &clr, float s)
    // {
    //     return clr / s;
    // }

    // static RGBSpectrum SpecSqrt(const RGBSpectrum &clr)
    // {
    //     RGBSpectrum result;

    //     result.r = std::sqrt(clr.r);
    //     result.g = std::sqrt(clr.g);
    //     result.b = std::sqrt(clr.b);

    //     return result;
    // }

    // static RGBSpectrum SpecPow(const RGBSpectrum &clr, float exp)
    // {
    //     RGBSpectrum result;

    //     result.r = std::pow(clr.r, exp);
    //     result.g = std::pow(clr.g, exp);
    //     result.b = std::pow(clr.b, exp);

    //     return result;
    // }

    // static RGBSpectrum GammaCorrect(RGBSpectrum &rgb)
    // {
    //     if (rgb.r >= 0.018f)
    //         rgb.r = (1.099f * powf(rgb.r, 0.45f)) - 0.099f;
    //     else
    //         rgb.r *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
    //     if (rgb.g >= 0.018f)
    //         rgb.g = (1.099f * powf(rgb.g, 0.45f)) - 0.099f;
    //     else
    //         rgb.g *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;
    //     if (rgb.b >= 0.018f)
    //         rgb.b = (1.099f * powf(rgb.b, 0.45f)) - 0.099f;
    //     else
    //         rgb.b *= ((1.099f * powf(0.018f, 0.45f)) - 0.099f) / 0.018f;

    //     // Return the gamma-corrected color
    //     return rgb;
    // }

} // namespace filianore

#endif