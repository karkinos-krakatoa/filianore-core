#ifndef _BSDF_H
#define _BSDF_H

#include "shadingframe.h"
#include "../maths/scalar.h"

namespace filianore
{

    enum BxDFType
    {
        BSDF_REFLECTION = 1 << 0,
        BSDF_REFRACTION = 1 << 1,

        BSDF_DIFFUSE = 1 << 2,
        BSDF_GLOSSY = 1 << 3,
        BSDF_SPECULAR = 1 << 4,

        BSDF_ALL = (1 << 5) - 1,
    };

    class BxDF
    {
    public:
        BxDF(const BxDFType &_bxdfType)
            : bxdfType(_bxdfType)
        {
        }

        virtual Color EvaluateBxDF(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const = 0;
        virtual Color SampleBxDF(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const = 0;
        virtual float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const = 0;

        bool MatchFlags(const BxDFType &t) const
        {
            return (bxdfType & t) == bxdfType;
        }

        const BxDFType bxdfType;
    };

    // BxDFs
    class LambertBxDF : public BxDF
    {
    public:
        LambertBxDF(const Color &_color)
            : color(_color), BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE))
        {
        }

        Color EvaluateBxDF(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
        {
            return color * InvPi<float>;
        }

        Color SampleBxDF(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const;

        float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
        {
            return ShadingFrame::SameHemisphere(wo, wi) ? ShadingFrame::AbsCosTheta(wi) * Inv2Pi<float> : 0;
        }

        Color color;
    };

    class BSDF
    {
    public:
        BSDF(const StaticArray<float, 3> &n, float _eta = 1);

        BSDF(const SurfaceInteraction &isect, float _eta = 1);

        void Add(BxDF *bxdf)
        {
            bxdfs[nBxDFs] = bxdf;
            nBxDFs++;
        }

        int NumComponents(const BxDFType &flags = BSDF_ALL) const;

        Color EvaluateBSDF(const StaticArray<float, 3> &woWorld, const StaticArray<float, 3> &wiWorld, const BxDFType &type = BSDF_ALL) const;

        Color SampleBSDF(const StaticArray<float, 3> &woWorld, StaticArray<float, 3> *wiWorld, const StaticArray<float, 2> &sample,
                         float *pdf, const BxDFType &type = BSDF_ALL, BxDFType *sampledType = nullptr) const;

        float Pdf(const StaticArray<float, 3> &woWorld, const StaticArray<float, 3> &wiWorld, const BxDFType &type = BSDF_ALL) const;

        float eta;

    private:
        ShadingFrame shadingFrame;
        static constexpr int MaxBxDFs = 4;
        int nBxDFs;
        BxDF *bxdfs[MaxBxDFs];
        const StaticArray<float, 3> ns, ng, ss, ts;
    };

} // namespace filianore

#endif