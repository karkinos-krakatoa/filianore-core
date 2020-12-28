#include "filianore/core/bsdf.h"
#include "filianore/core/sampling.h"

namespace filianore
{

    Color LambertBxDF::SampleBxDF(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        *wi = CosineSampleHemisphere(sample);
        if (wo.z() < 0)
        {
            wi->params[2] *= -1;
        }
        *pdf = Pdf(wo, *wi);
        return EvaluateBxDF(wo, *wi);
    }

    int BSDF::NumComponents(const BxDFType &flags) const
    {
        int num = 0;
        for (int i = 0; i < nBxDFs; i++)
        {
            if (bxdfs[i]->MatchFlags(flags))
            {
                num++;
            }
        }
        return num;
    }

    Color BSDF::EvaluateBSDF(const StaticArray<float, 3> &woWorld, const StaticArray<float, 3> &wiWorld, const BxDFType &type) const
    {
        StaticArray<float, 3> woLocal = shadingFrame.ToLocal(woWorld);
        StaticArray<float, 3> wiLocal = shadingFrame.ToLocal(wiWorld);

        bool reflect = Dot(shadingFrame.n, wiWorld) * Dot(shadingFrame.n, woWorld) > 0.f;

        Color f(0.f);

        for (int i = 0; i < nBxDFs; i++)
        {
            if (bxdfs[i]->MatchFlags(type) &&
                ((reflect && (bxdfs[i]->bxdfType & BxDFType::BSDF_REFLECTION)) ||
                 (!reflect && (bxdfs[i]->bxdfType & BxDFType::BSDF_REFRACTION))))
            {
                f += bxdfs[i]->EvaluateBxDF(woLocal, wiLocal);
            }
        }

        return f;
    }

    Color BSDF::SampleBSDF(const StaticArray<float, 3> &woWorld, StaticArray<float, 3> *wiWorld, const StaticArray<float, 2> &sample,
                           float *pdf, const BxDFType &type, BxDFType *sampledType) const
    {
        // Choose BxDFs to sample
        int matchingComps = NumComponents(type);
        if (matchingComps == 0)
        {
            *pdf = 0.f;
            return Color(0.f);
        }
        int comp = std::min((int)std::floor(sample.x() * matchingComps), matchingComps - 1);

        // Get BxDf
        BxDF *bxdf = nullptr;
        int count = comp;
        for (int i = 0; i < nBxDFs; i++)
        {
            if (bxdfs[i]->MatchFlags(type) && count-- == 0)
            {
                bxdf = bxdfs[i];
                break;
            }
        }

        // Remap
        StaticArray<float, 2> uRemapped(sample.x() * matchingComps - comp, sample.y());

        // Sample Chosen BxDF
        StaticArray<float, 3> wi, wo = shadingFrame.ToLocal(woWorld);
        *pdf = 0.f;
        if (sampledType)
        {
            *sampledType = bxdf->bxdfType;
        }
        Color f = bxdf->SampleBxDF(wo, &wi, uRemapped, pdf, sampledType);
        if (pdf == 0)
        {
            return Color(0.f);
        }
        *wiWorld = shadingFrame.ToWorld(wi);

        if (!(bxdf->bxdfType & BSDF_SPECULAR) && matchingComps > 1)
        {
            for (int i = 0; i < nBxDFs; i++)
            {
                if (bxdfs[i] != bxdf && bxdfs[i]->MatchFlags(type))
                {
                    *pdf += bxdfs[i]->Pdf(wo, wi);
                }
            }
        }

        if (matchingComps > 1)
        {
            *pdf /= (float)matchingComps;
        }

        if (!(bxdf->bxdfType & BSDF_SPECULAR) && matchingComps > 1)
        {
            bool reflect = Dot(*wiWorld, ng) * Dot(woWorld, ng) > 0;
            f = Color(0.f);
            for (int i = 0; i < nBxDFs; ++i)
                if (bxdfs[i]->MatchFlags(type) &&
                    ((reflect && (bxdfs[i]->bxdfType & BSDF_REFLECTION)) ||
                     (!reflect && (bxdfs[i]->bxdfType & BSDF_REFRACTION))))
                    f += bxdfs[i]->EvaluateBxDF(wo, wi);
        }

        return f;
    }

    float BSDF::Pdf(const StaticArray<float, 3> &woWorld, const StaticArray<float, 3> &wiWorld, const BxDFType &type) const
    {
        float pdf = 0.f;

        if (nBxDFs == 0)
        {
            return pdf;
        }

        StaticArray<float, 3> woLocal = shadingFrame.ToLocal(woWorld);
        StaticArray<float, 3> wiLocal = shadingFrame.ToLocal(wiWorld);

        int matchComp = 0;

        for (int i = 0; i < nBxDFs; i++)
        {
            if (bxdfs[i]->MatchFlags(type))
            {
                pdf += bxdfs[i]->Pdf(woLocal, wiLocal);
                matchComp++;
            }
        }

        return matchComp > 0 ? pdf / matchComp : 0;
    }

} // namespace filianore