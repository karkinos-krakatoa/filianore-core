#include "filianore/shading/microfacetreflection.h"
#include "filianore/core/microfacet.h"
#include "filianore/core/fresnel.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    PrincipalSpectrum MicrofacetReflection::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        float cosThetaO = AbsCosTheta(wo);
        float cosThetaI = AbsCosTheta(wi);

        StaticArray<float, 3> wh = wi + wo;

        if (cosThetaI == 0 || cosThetaO == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        if (wh.x() == 0 && wh.y() == 0 && wh.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        wh = wh.Normalize();

        PrincipalSpectrum F = fresnel->Evaluate(Dot(wo, wh));

        return R * distribution->EvaluateD(wh) * distribution->EvaluateG(wo, wi) * F / (4.f * cosThetaO * cosThetaI);
    }

    PrincipalSpectrum MicrofacetReflection::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        if (wo.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        StaticArray<float, 3> wh = distribution->SampleWh(wo, sample);
        if (Dot(wo, wh) < 0)
        {
            return PrincipalSpectrum(0.f);
        }

        *wi = Reflect(wo, wh);
        if (!SameHemisphere(wo, *wi))
        {
            return PrincipalSpectrum(0.f);
        }

        *pdf = distribution->Pdf(wo, wh) / (4 * Dot(wo, wh));

        return Evaluate(wo, *wi);
    }

    float MicrofacetReflection::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (!SameHemisphere(wo, wi))
        {
            return 0.f;
        }
        StaticArray<float, 3> wh = (wo + wi).Normalize();
        return distribution->Pdf(wo, wh) / (4.f * Dot(wo, wh));
    }

}