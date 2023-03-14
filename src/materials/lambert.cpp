#include "filianore/materials/lambert.h"
#include "filianore/core/bsdf.h"
#include "filianore/core/interaction.h"
#include "filianore/core/texture.h"
#include "filianore/shading/bxdfs/lambert.h"

namespace filianore {

void LambertMaterial::compute_scattering_functions(SurfaceInteraction *isect) const {
    isect->bsdf = std::make_shared<BSDF>(*isect);

    PrincipalSpectrum r = kd->evaluate(*isect);
    r = r.spectrum_clamp();

    std::unique_ptr<BxDF> lambRefl = std::make_unique<LambertBRDF>(r, 1.f);
    isect->bsdf->add(lambRefl);
}

} // namespace filianore
