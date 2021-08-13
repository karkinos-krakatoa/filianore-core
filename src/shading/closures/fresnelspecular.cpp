#include "filianore/shading/closures/fresnelspecular.h"
#include "filianore/core/microfacet.h"
#include "filianore/shading/fresnel/fresneldielectric.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    FresnelSpecularBXDF::FresnelSpecularBXDF(const PrincipalSpectrum &_R, const PrincipalSpectrum &_T, float _etaA, float _etaB)
        : R(_R), T(_T), etaA(_etaA), etaB(_etaB),
          BxDF(BxDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR))
    {
        fresnel = std::make_shared<FresnelDielectric>(etaA, etaB);
    }

    PrincipalSpectrum FresnelSpecularBXDF::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return PrincipalSpectrum(0.f);
    }

    PrincipalSpectrum FresnelSpecularBXDF::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        float F = fresnel->Evaluate(CosTheta(wo)).c[0];

        if (sample.x() < F)
        {
            *wi = StaticArray<float, 3>(-wo.x(), -wo.y(), wo.z());
            if (sampledType)
            {
                *sampledType = BxDFType(BSDF_SPECULAR | BSDF_REFLECTION);
            }

            *pdf = F;
            return R * F / AbsCosTheta(*wi);
        }
        else
        {
            bool entering = CosTheta(wo) > 0;
            float etaI = entering ? etaA : etaB;
            float etaT = entering ? etaB : etaA;

            // Compute ray direction for specular transmission
            if (!Refract(wo, Faceforward(StaticArray<float, 3>(0.f, 0.f, 1.f), wo), etaI / etaT, wi))
            {
                return 0.f;
            }
            PrincipalSpectrum ft = T * (1.f - F);

            ft *= (etaI * etaI) / (etaT * etaT);

            if (sampledType)
            {
                *sampledType = BxDFType(BSDF_SPECULAR | BSDF_TRANSMISSION);
            }

            *pdf = 1.f - F;
            return ft / AbsCosTheta(*wi);
        }
    }

    float FresnelSpecularBXDF::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

}