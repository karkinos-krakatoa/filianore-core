#ifndef _BSDF_H
#define _BSDF_H

#include "bxdf.h"
#include <memory>
#include <vector>

namespace filianore {

class BSDF {
public:
    BSDF(const SurfaceInteraction &isect, float _eta = 1.f);

    ~BSDF() {}

    void add(std::unique_ptr<BxDF> &_bxdf) {
        bxdfs.emplace_back(std::move(_bxdf));
        nBxDFs++;
    }

    int num_components(BxDFType flags = BSDF_ALL) const;

    Vector3f to_local(const Vector3f &v) const;

    Vector3f to_world(const Vector3f &v) const;

    PrincipalSpectrum evaluate(const Vector3f &woW, const Vector3f &wiW, BxDFType flags = BSDF_ALL) const;

    PrincipalSpectrum sample(const Vector3f &woW, Vector3f *wiW, const Vector2f &u, float *pdf,
                             BxDFType flags = BSDF_ALL, BxDFType *sampledType = nullptr) const;

    float pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags = BSDF_ALL) const;

    const float eta;

private:
    const Vector3f ng, ns;
    Vector3f s, t;

    int nBxDFs = 0;
    static constexpr int MaxBxDFs = 8;
    std::vector<std::unique_ptr<BxDF>> bxdfs;
};

} // namespace filianore

#endif