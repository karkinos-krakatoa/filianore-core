#include "filianore/shading/closures/fresnelblendeddiffspec.h"
#include "filianore/core/shadingcore.h"
#include "filianore/core/microfacet.h"
#include "filianore/core/fresnel.h"

namespace filianore
{

    FresnelBlendedDiffuseSpecularBRDF::FresnelBlendedDiffuseSpecularBRDF(const PrincipalSpectrum &_kd, float _kdWeight, float _kdRoughness,

                                                                         const PrincipalSpectrum &_ks, float _ksWeight, float _ro,

                                                                         const std::shared_ptr<Fresnel> &_fresnel,
                                                                         const std::shared_ptr<MicrofacetDistribution> &_distribution)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)),
          kd(_kd), kdWeight(_kdWeight), kdRoughness(_kdRoughness),
          ks(_ks), ksWeight(_ksWeight), ro(_ro),
          fresnel(_fresnel), distribution(_distribution)
    {
        orenNayarBrdf = std::make_shared<OrenNayarBRDF>(kd, kdWeight, kdRoughness);
    }

    PrincipalSpectrum FresnelBlendedDiffuseSpecularBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        // Diffuse
        float r1 = 1.f - .5f * AbsCosTheta(wi);
        float r1pow5 = (r1 * r1) * (r1 * r1) * r1;
        float r2 = 1.f - .5f * AbsCosTheta(wo);
        float r2pow5 = (r2 * r2) * (r2 * r2) * r2;

        PrincipalSpectrum diffuse = (28.f / 23.f) * orenNayarBrdf->Evaluate(wo, wi) * (1.f - ro) * (1 - r1pow5) * (1 - r2pow5);

        // Specular
        StaticArray<float, 3> wh = wi + wo;
        if (wh.x() == 0 && wh.y() == 0 && wh.z() == 0)
        {
            return PrincipalSpectrum(0);
        }
        wh = wh.Normalize();

        PrincipalSpectrum specular = distribution->EvaluateD(wh) * fresnel->Evaluate(Dot(wi, wh)) / (4.f * AbsDot(wi, wh) * std::max(AbsCosTheta(wi), AbsCosTheta(wo)));
        specular = ks * specular * ksWeight;

        return diffuse + specular;
    }

    PrincipalSpectrum FresnelBlendedDiffuseSpecularBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        StaticArray<float, 2> u = sample;
        if (u.x() < .5)
        {
            u.params[0] = std::min(2 * u.x(), OneMinusEpsilon<float>);
            *wi = CosineHemisphereSample(u);
            if (wo.z() < 0)
            {
                wi->params[2] *= -1.f;
            }
        }
        else
        {
            u.params[0] = std::min(2 * (u.x() - .5f), OneMinusEpsilon<float>);
            StaticArray<float, 3> wh = distribution->SampleWh(wo, u);
            *wi = Reflect(wo, wh);
            if (!SameHemisphere(wo, *wi))
                return PrincipalSpectrum(0.f);
        }
        *pdf = Pdf(wo, *wi);
        return Evaluate(wo, *wi);
    }

    float FresnelBlendedDiffuseSpecularBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (!SameHemisphere(wo, wi))
            return 0;
        StaticArray<float, 3> wh = (wo + wi).Normalize();
        float pdf_wh = distribution->Pdf(wo, wh);
        return .5f * (AbsCosTheta(wi) * InvPi<float> + pdf_wh / (4 * Dot(wo, wh)));
    }

} // namespace filianore