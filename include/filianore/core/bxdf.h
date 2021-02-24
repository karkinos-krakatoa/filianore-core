#ifndef _BXDF_H
#define _BXDF_H

#include "../maths/static_array.h"
#include "../color/principalspectrum.h"

namespace filianore
{
    enum BxDFType
    {
        BSDF_REFLECTION = 1 << 0,
        BSDF_TRANSMISSION = 1 << 1,
        BSDF_DIFFUSE = 1 << 2,
        BSDF_GLOSSY = 1 << 3,
        BSDF_SPECULAR = 1 << 4,
        BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
    };

    class BxDF
    {
    public:
        virtual ~BxDF() {}

        BxDF(BxDFType type)
            : bxDFType(type)
        {
        }

        bool MatchesFlags(BxDFType t) const
        {
            return (bxDFType & t) == bxDFType;
        }

        virtual PrincipalSpectrum Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const = 0;

        virtual PrincipalSpectrum Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const = 0;

        virtual float Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const = 0;

        const BxDFType bxDFType;
    };

} // namespace filianore

#endif