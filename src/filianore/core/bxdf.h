#ifndef _BXDF_H
#define _BXDF_H

#include "../color/principalspectrum.h"
#include "../maths/vec.h"

namespace filianore {
enum BxDFType {
    BSDF_REFLECTION = 1 << 0,
    BSDF_TRANSMISSION = 1 << 1,
    BSDF_DIFFUSE = 1 << 2,
    BSDF_GLOSSY = 1 << 3,
    BSDF_SPECULAR = 1 << 4,
    BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

class BxDF {
public:
    virtual ~BxDF() {}

    BxDF(BxDFType type)
        : bxDFType(type) {
    }

    bool matches_flags(BxDFType t) const {
        return (bxDFType & t) == bxDFType;
    }

    virtual PrincipalSpectrum evaluate(const Vector3f &wo, const Vector3f &wi) const = 0;

    virtual PrincipalSpectrum sample(const Vector3f &wo, Vector3f *wi, const Vector2f &sample, float *pdf, BxDFType *sampledType) const = 0;

    virtual float pdf(const Vector3f &wo, const Vector3f &wi) const = 0;

    const BxDFType bxDFType;
};

} // namespace filianore

#endif