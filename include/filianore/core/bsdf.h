#ifndef _BSDF_H
#define _BSDF_H

#include "bxdf.h"
#include "../maths/vec3_math.h"

namespace filianore
{

    class BSDF
    {
    public:
        BSDF(const SurfaceInteraction &isect, float _eta = 1.f);

        ~BSDF() {}

        void Add(BxDF *b)
        {
            bxdfs[nBxDFs++] = b;
        }

        int NumComponents(BxDFType flags = BSDF_ALL) const;

        StaticArray<float, 3> ToLocal(const StaticArray<float, 3> &v) const;

        StaticArray<float, 3> ToWorld(const StaticArray<float, 3> &v) const;

        PrincipalSpectrum Evaluate(const StaticArray<float, 3> &woW, const StaticArray<float, 3> &wiW, BxDFType flags = BSDF_ALL) const;

        PrincipalSpectrum Sample(const StaticArray<float, 3> &woW, StaticArray<float, 3> *wiW, const StaticArray<float, 2> &u, float *pdf,
                                 BxDFType flags = BSDF_ALL, BxDFType *sampledType = nullptr) const;

        float Pdf(const StaticArray<float, 3> &woW, const StaticArray<float, 3> &wiW, BxDFType flags = BSDF_ALL) const;

        const float eta;

    private:
        const StaticArray<float, 3> ng, ns;
        StaticArray<float, 3> s, t;

        int nBxDFs = 0;
        static constexpr int MaxBxDFs = 8;
        BxDF *bxdfs[MaxBxDFs];
    };

} // namespace filianore

#endif