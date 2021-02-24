#include "filianore/core/bsdf.h"
#include "filianore/core/interaction.h"

namespace filianore
{

    BSDF::BSDF(const SurfaceInteraction &isect, float _eta)
        : eta(_eta), ng(isect.n), ns(isect.shading.n)
    {
        CoordinateSystem<float>(ns, &s, &t);
        s = s.Normalize();
    }

    int BSDF::NumComponents(BxDFType flags) const
    {
        int num = 0;
        for (int i = 0; i < nBxDFs; ++i)
        {
            if (bxdfs[i]->MatchesFlags(flags))
            {
                ++num;
            }
        }
        return num;
    }

    StaticArray<float, 3> BSDF::ToLocal(const StaticArray<float, 3> &v) const
    {
        return StaticArray<float, 3>(Dot(v, s), Dot(v, t), Dot(v, ns));
    }

    StaticArray<float, 3> BSDF::ToWorld(const StaticArray<float, 3> &v) const
    {
        return (s * v.x() + t * v.y() + ns * v.z());
    }

    PrincipalSpectrum BSDF::Evaluate(const StaticArray<float, 3> &woW, const StaticArray<float, 3> &wiW, BxDFType flags) const
    {
        StaticArray<float, 3> wi = ToLocal(wiW);
        StaticArray<float, 3> wo = ToLocal(woW);

        if (wo.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        bool reflect = Dot(wiW, ng) * Dot(woW, ng) > 0;

        PrincipalSpectrum bsdf_total(0);
        for (int i = 0; i < nBxDFs; ++i)
        {
            if (bxdfs[i]->MatchesFlags(flags) &&
                ((reflect && (bxdfs[i]->bxDFType & BSDF_REFLECTION)) ||
                 (!reflect && (bxdfs[i]->bxDFType & BSDF_TRANSMISSION))))
            {
                bsdf_total += bxdfs[i]->Evaluate(wo, wi);
            }
        }

        return bsdf_total;
    }

    PrincipalSpectrum BSDF::Sample(const StaticArray<float, 3> &woW, StaticArray<float, 3> *wiW, const StaticArray<float, 2> &u, float *pdf,
                                   BxDFType flags, BxDFType *sampledType) const
    {
        int matchComps = NumComponents(flags);
        if (matchComps == 0)
        {
            *pdf = 0.f;
            if (sampledType)
            {
                *sampledType = BxDFType(0);
            }
            return PrincipalSpectrum(0.f);
        }

        int comp = std::min((int)std::floor(u.x() * matchComps), matchComps - 1);

        BxDF *bxdf = nullptr;
        int count = comp;
        for (int i = 0; i < nBxDFs; ++i)
        {
            if (bxdfs[i]->MatchesFlags(flags) && count-- == 0)
            {
                bxdf = bxdfs[i];
                break;
            }
        }

        // Resampled u - This is because the u[0] was used above - so it's no longer uniformly distributed
        StaticArray<float, 2> uReadjusted(std::min(u.x() * matchComps - comp, 1.f - Epsilon<float>), u.y());

        StaticArray<float, 3> wi, wo = ToLocal(woW);
        if (wo.z() == 0)
        {
            return PrincipalSpectrum(0.f);
        }

        *pdf = 0.f;

        if (sampledType)
        {
            *sampledType = bxdf->bxDFType;
        }

        PrincipalSpectrum f = bxdf->Sample(wo, &wi, uReadjusted, pdf, sampledType);

        if (*pdf == 0)
        {
            if (sampledType)
            {
                *sampledType = BxDFType(0);
            }
            return PrincipalSpectrum(0.f);
        }

        *wiW = ToWorld(wi);

        if (!(bxdf->bxDFType & BSDF_SPECULAR) && matchComps > 1)
        {
            for (int i = 0; i < nBxDFs; ++i)
            {
                if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(flags))
                {
                    *pdf += bxdfs[i]->Pdf(wo, wi);
                }
            }
        }

        if (matchComps > 1)
        {
            *pdf /= matchComps;
        }

        if (!(bxdf->bxDFType & BSDF_SPECULAR))
        {
            bool reflect = Dot(*wiW, ng) * Dot(woW, ng) > 0;
            f = PrincipalSpectrum(0.f);

            for (int i = 0; i < nBxDFs; ++i)
            {
                if (bxdfs[i]->MatchesFlags(flags) &&
                    ((reflect && (bxdfs[i]->bxDFType & BSDF_REFLECTION)) ||
                     (!reflect && (bxdfs[i]->bxDFType & BSDF_TRANSMISSION))))
                {
                    f += bxdfs[i]->Evaluate(wo, wi);
                }
            }
        }

        return f;
    }

    float BSDF::Pdf(const StaticArray<float, 3> &woW, const StaticArray<float, 3> &wiW, BxDFType flags) const
    {
        if (nBxDFs == 0)
        {
            return 0.f;
        }

        StaticArray<float, 3> wi = ToLocal(wiW);
        StaticArray<float, 3> wo = ToLocal(woW);

        if (wo.z() == 0)
        {
            return 0.f;
        }

        float pdf = 0.f;
        int matchingComps = 0;

        for (int i = 0; i < nBxDFs; ++i)
        {
            if (bxdfs[i]->MatchesFlags(flags))
            {
                ++matchingComps;
                pdf += bxdfs[i]->Pdf(wo, wi);
            }
        }

        float pdfEval = matchingComps > 0 ? pdf / matchingComps : 0.f;
        return pdfEval;
    }

} // namespace filianore