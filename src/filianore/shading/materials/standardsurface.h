#ifndef _STANDARD_SURFACE_MATERIAL_H
#define _STANDARD_SURFACE_MATERIAL_H

#include "../../core/elemental.h"
#include "material.h"

namespace filianore {

enum class FresnelType : int {
    Dielectric = 0,
    Metallic = 1,
    WavelengthDependentMetallic = 2
};

class StandardSurfaceMaterial : public Material {
public:
    StandardSurfaceMaterial(const float _kdweight,
                            const std::shared_ptr<Texture<PrincipalSpectrum>> &_kd,
                            const std::shared_ptr<Texture<float>> &_kdroughness,

                            const float _ksweight,
                            const std::shared_ptr<Texture<PrincipalSpectrum>> &_ks,
                            const std::shared_ptr<Texture<float>> &_ksroughness,
                            const std::shared_ptr<Texture<float>> &_ksanisotropic,
                            const int _fresnelType,
                            const float _ksIOR,
                            const int _metalType,

                            const float _sheenweight,
                            const std::shared_ptr<Texture<PrincipalSpectrum>> &_sheenColor,
                            const std::shared_ptr<Texture<float>> &_sheenroughness,

                            const float _ktweight,
                            const std::shared_ptr<Texture<PrincipalSpectrum>> &_kt,

                            const float _krcoatweight,
                            const std::shared_ptr<Texture<PrincipalSpectrum>> &_krcoat,
                            const float _krcoatior,
                            const float _krcoatgloss,

                            const float _thinFilmThickness,
                            const float _thinFilmIOR,

                            const bool _thinWalled,
                            const std::shared_ptr<Texture<float>> &_bumpMap);

    void compute_scattering_functions(SurfaceInteraction *isect) const;

private:
    // Diffuse
    const std::shared_ptr<Texture<PrincipalSpectrum>> kd;
    const std::shared_ptr<Texture<float>> kdroughness;
    const float kdweight;

    // Specular
    const std::shared_ptr<Texture<PrincipalSpectrum>> ks;
    const std::shared_ptr<Texture<float>> ksroughness;
    const std::shared_ptr<Texture<float>> ksanisotropic;
    const float ksweight;
    const int fresnelType;
    const float ksIOR;
    const int metalType;

    // Sheen
    const std::shared_ptr<Texture<PrincipalSpectrum>> sheenColor;
    const std::shared_ptr<Texture<float>> sheenroughness;
    const float sheenweight;

    // Transmission
    const std::shared_ptr<Texture<PrincipalSpectrum>> kt;
    const float ktweight;

    // Clearcoat
    const float krcoatweight;
    const std::shared_ptr<Texture<PrincipalSpectrum>> krcoat;
    const float krcoatior;
    const float krcoatgloss;

    // Thin Film
    const float thinFilmThickness;
    const float thinFilmIOR;

    // Geometry
    const float thinWalled;
    const std::shared_ptr<Texture<float>> bumpMap;
};

} // namespace filianore

#endif