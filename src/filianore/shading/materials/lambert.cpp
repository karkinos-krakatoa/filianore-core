#include "lambert.h"
#include "../../core/interaction.h"
#include "../bsdf.h"
#include "../bxdfs/lambert.h"
#include "../textures/texture.h"

namespace filianore {

void LambertMaterial::compute_scattering_functions(SurfaceInteraction *isect) const {
    isect->bsdf = std::make_shared<BSDF>(*isect);

    PrincipalSpectrum r = kd->evaluate(*isect);
    r = r.spectrum_clamp();

    std::unique_ptr<BxDF> lambRefl = std::make_unique<LambertBRDF>(r, 1.f);
    isect->bsdf->add(lambRefl);
}

} // namespace filianore
