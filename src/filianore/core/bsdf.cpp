#include "bsdf.h"
#include "interaction.h"

namespace filianore {

BSDF::BSDF(const SurfaceInteraction &isect, float _eta)
    : eta(_eta), ng(isect.n), ns(isect.Shading.n) {
    coordinate_system(ns, &s, &t);
    s = normalize(s);
}

int BSDF::num_components(BxDFType flags) const {
    int num = 0;
    for (int i = 0; i < nBxDFs; ++i) {
        if (bxdfs[i]->matches_flags(flags)) {
            ++num;
        }
    }
    return num;
}

Vector3f BSDF::to_local(const Vector3f &v) const {
    return Vector3f(dot(v, s), dot(v, t), dot(v, ns));
}

Vector3f BSDF::to_world(const Vector3f &v) const {
    return (s * v.x + t * v.y + ns * v.z);
}

PrincipalSpectrum BSDF::evaluate(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const {
    Vector3f wi = to_local(wiW);
    Vector3f wo = to_local(woW);

    if (wo.z == 0) {
        return PrincipalSpectrum(0.f);
    }

    bool reflect = dot(wiW, ng) * dot(woW, ng) > 0;

    PrincipalSpectrum bsdf_total(0);
    for (int i = 0; i < nBxDFs; ++i) {
        if (bxdfs[i]->matches_flags(flags) &&
            ((reflect && (bxdfs[i]->bxDFType & BSDF_REFLECTION)) ||
             (!reflect && (bxdfs[i]->bxDFType & BSDF_TRANSMISSION)))) {
            bsdf_total += bxdfs[i]->evaluate(wo, wi);
        }
    }

    return bsdf_total;
}

PrincipalSpectrum BSDF::sample(const Vector3f &woW, Vector3f *wiW, const Vector2f &u, float *pdf,
                               BxDFType flags, BxDFType *sampledType) const {
    int matchComps = num_components(flags);
    if (matchComps == 0) {
        *pdf = 0.f;
        if (sampledType) {
            *sampledType = BxDFType(0);
        }
        return PrincipalSpectrum(0.f);
    }

    int comp = std::min((int)std::floor(u.x * matchComps), matchComps - 1);

    BxDF *bxdf = nullptr;
    int count = comp;
    for (int i = 0; i < nBxDFs; ++i) {
        if (bxdfs[i]->matches_flags(flags) && count-- == 0) {
            bxdf = bxdfs[i].get();
            break;
        }
    }

    // Resampled u - This is because the u[0] was used above - so it's no longer uniformly distributed
    Vector2f uReadjusted(std::min(u.x * matchComps - comp, 1.f - SHADOW_EPSILON), u.y);

    Vector3f wi, wo = to_local(woW);
    if (wo.z == 0) {
        return PrincipalSpectrum(0.f);
    }

    *pdf = 0.f;

    if (sampledType) {
        *sampledType = bxdf->bxDFType;
    }

    PrincipalSpectrum f = bxdf->sample(wo, &wi, uReadjusted, pdf, sampledType);

    if (*pdf == 0) {
        if (sampledType) {
            *sampledType = BxDFType(0);
        }
        return PrincipalSpectrum(0.f);
    }

    *wiW = to_world(wi);

    if (!(bxdf->bxDFType & BSDF_SPECULAR) && matchComps > 1) {
        for (int i = 0; i < nBxDFs; ++i) {
            if (bxdfs[i].get() != bxdf && bxdfs[i]->matches_flags(flags)) {
                *pdf += bxdfs[i]->pdf(wo, wi);
            }
        }
    }

    if (matchComps > 1) {
        *pdf /= matchComps;
    }

    if (!(bxdf->bxDFType & BSDF_SPECULAR)) {
        bool reflect = dot(*wiW, ng) * dot(woW, ng) > 0;
        f = PrincipalSpectrum(0.f);

        for (int i = 0; i < nBxDFs; ++i) {
            if (bxdfs[i]->matches_flags(flags) &&
                ((reflect && (bxdfs[i]->bxDFType & BSDF_REFLECTION)) ||
                 (!reflect && (bxdfs[i]->bxDFType & BSDF_TRANSMISSION)))) {
                f += bxdfs[i]->evaluate(wo, wi);
            }
        }
    }

    return f;
}

float BSDF::pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const {
    if (nBxDFs == 0) {
        return 0.f;
    }

    Vector3f wi = to_local(wiW);
    Vector3f wo = to_local(woW);

    if (wo.z == 0) {
        return 0.f;
    }

    float pdf = 0.f;
    int matchingComps = 0;

    for (int i = 0; i < nBxDFs; ++i) {
        if (bxdfs[i]->matches_flags(flags)) {
            ++matchingComps;
            pdf += bxdfs[i]->pdf(wo, wi);
        }
    }

    float pdfEval = matchingComps > 0 ? pdf / matchingComps : 0.f;
    return pdfEval;
}

} // namespace filianore