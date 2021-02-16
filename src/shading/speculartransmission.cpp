#include "filianore/shading/speculartransmission.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    RGBSpectrum SpecularTransmission::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return RGBSpectrum(0.f);
    }

    RGBSpectrum SpecularTransmission::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        bool entering = CosTheta(wo) > 0;
        float etaI = entering ? etaA : etaB;
        float etaT = entering ? etaB : etaA;

        if (!Refract(wo, Faceforward(StaticArray<float, 3>(0.f, 0.f, 1.f), wo), etaI / etaT, wi))
        {
            return RGBSpectrum(0.f);
        }

        *pdf = 1.f;
        RGBSpectrum ft = T * (RGBSpectrum(1.f) - fresnel.Evaluate(CosTheta(*wi)));

        // Account for non-symmetry with transmission to different medium
        ft *= (etaI * etaI) / (etaT * etaT);
        return ft / AbsCosTheta(*wi);
    }

    float SpecularTransmission::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

} // namespace filianore