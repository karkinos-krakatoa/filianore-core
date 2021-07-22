#include "filianore/shading/bxdfs/plastic.h"
#include "filianore/core/shadingcore.h"
#include "filianore/core/microfacet.h"

namespace filianore
{

    PlasticBRDF::PlasticBRDF(float _kdWeight, const PrincipalSpectrum &_kd,
                             float _ksWeight, const PrincipalSpectrum &_ks, float _ro,
                             const std::shared_ptr<MicrofacetDistribution> &_distribution)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)),
          kdWeight(_kdWeight), kd(_kd),
          ksWeight(_ksWeight), ks(_ks), ro(_ro),
          distribution(_distribution)
    {
    }

    PrincipalSpectrum PlasticBRDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        auto pow5 = [](float v)
        { return (v * v) * (v * v) * v; };

        // Diffuse
        PrincipalSpectrum diffuse = (28.f / (23.f * Pi<float>)) * kd * (PrincipalSpectrum(1.f) - ro) *
                                    (1 - pow5(1 - .5f * AbsCosTheta(wi))) *
                                    (1 - pow5(1 - .5f * AbsCosTheta(wo)));
        diffuse = diffuse * kdWeight;

        // Specular
        StaticArray<float, 3> wh = wi + wo;
        if (wh.x() == 0 && wh.y() == 0 && wh.z() == 0)
        {
            return PrincipalSpectrum(0);
        }
        wh = wh.Normalize();

        PrincipalSpectrum specular = distribution->EvaluateD(wh) * SchlickFresnel(Dot(wi, wh)) / (4.f * AbsDot(wi, wh) * std::max(AbsCosTheta(wi), AbsCosTheta(wo)));
        specular = specular * ksWeight;

        return diffuse + specular;
    }

    PrincipalSpectrum PlasticBRDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
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

    float PlasticBRDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        if (!SameHemisphere(wo, wi))
            return 0;
        StaticArray<float, 3> wh = (wo + wi).Normalize();
        float pdf_wh = distribution->Pdf(wo, wh);
        return .5f * (AbsCosTheta(wi) * InvPi<float> + pdf_wh / (4 * Dot(wo, wh)));
    }

} // namespace filianore