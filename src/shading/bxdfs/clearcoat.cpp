#include "filianore/shading/bxdfs/clearcoat.h"
#include "filianore/core/microfacet.h"
#include "filianore/shading/fresnel/fresneldielectric.h"
#include "filianore/core/shadingcore.h"
#include "filianore/maths/vec3_math.h"

namespace filianore
{
    inline float GTR1(float cosTheta, float alpha)
    {
        float alpha2 = alpha * alpha;
        return (alpha2 - 1) /
               (Pi<float> * std::log(alpha2) * (1 + (alpha2 - 1) * cosTheta * cosTheta));
    }

    // Smith masking/shadowing term.
    inline float smithG_GGX(float cosTheta, float alpha)
    {
        float alpha2 = alpha * alpha;
        float cosTheta2 = cosTheta * cosTheta;
        return 1.f / (cosTheta + sqrt(alpha2 + cosTheta2 - alpha2 * cosTheta2));
    }

    ClearcoatReflectionBRDF::ClearcoatReflectionBRDF(const PrincipalSpectrum &_R, float _ior, float _weight, float _gloss)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), R(_R), ior(_ior), weight(_weight), gloss(_gloss)
    {
        fresnel = std::make_shared<FresnelDielectric>(1.f, ior);
        gloss = Lerp<float>(gloss, .1f, .001f);
    }

    PrincipalSpectrum ClearcoatReflectionBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        StaticArray<float, 3> wh = wi + wo;
        if (wh.x() == 0 && wh.y() == 0 && wh.z() == 0)
        {
            return PrincipalSpectrum(0.);
        }

        wh = wh.Normalize();

        float Dr = GTR1(AbsCosTheta(wh), gloss);
        PrincipalSpectrum Fr = fresnel->Evaluate(Dot(wo, wh));

        // The geometric term always based on alpha = 0.25.
        float Gr = smithG_GGX(AbsCosTheta(wo), .25) * smithG_GGX(AbsCosTheta(wi), .25);

        return R * Fr * weight * Gr * Dr / 4;
    }

    PrincipalSpectrum ClearcoatReflectionBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        if (wo.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        float alpha2 = gloss * gloss;
        float cosTheta = std::sqrt(std::max(0.f, (1 - std::pow(alpha2, 1 - sample.x())) / (1 - alpha2)));
        float sinTheta = std::sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
        float phi = 2 * Pi<float> * sample.y();

        StaticArray<float, 3> wh = SphericalDirection<float>(sinTheta, cosTheta, phi);
        if (!SameHemisphere(wo, wh))
        {
            wh = wh.Neg();
        }

        *wi = Reflect(wo, wh);
        if (!SameHemisphere(wo, *wi))
        {
            return PrincipalSpectrum(0.f);
        }

        *pdf = Pdf(wo, *wi);
        return Evaluate(wo, *wi);
    }

    float ClearcoatReflectionBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (!SameHemisphere(wo, wi))
            return 0;

        StaticArray<float, 3> wh = wi + wo;
        if (wh.x() == 0 && wh.y() == 0 && wh.z() == 0)
            return 0;

        wh = wh.Normalize();

        float Dr = GTR1(AbsCosTheta(wh), gloss);
        return Dr * AbsCosTheta(wh) / (4 * Dot(wo, wh));
    }

}