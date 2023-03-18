#include "material.h"
#include "../core/interaction.h"
#include "../core/primitive.h"
#include "../textures/imagemap.h"

namespace filianore {

void Material::bump_evaluate(const std::shared_ptr<Texture<float>> &bumpMap, SurfaceInteraction *si) {
    SurfaceInteraction siEval = *si;

    float du = 0.0005f, dv = 0.0005f;

    // Shift si in u-direction
    siEval.p = si->p + si->dpdu * du;
    siEval.uv = si->uv + Vector2f(du, 0.f);
    siEval.n = normalize(cross(si->Shading.dpdu, si->Shading.dpdv));
    float uDisplace = bumpMap->evaluate(siEval);

    // Shift si in v-direction
    siEval.p = si->p + si->dpdv * dv;
    siEval.uv = si->uv + Vector2f(0.f, dv);
    siEval.n = normalize(cross(si->Shading.dpdu, si->Shading.dpdv));
    float vDisplace = bumpMap->evaluate(siEval);

    float displace = bumpMap->evaluate(*si);

    Vector3f dpdu = si->Shading.dpdu + si->Shading.n * ((uDisplace - displace) / du) * 4.f;
    Vector3f dpdv = si->Shading.dpdv + si->Shading.n * ((vDisplace - displace) / dv) * 4.f;

    si->set_shading_geometry(dpdu, dpdv, si->Shading.dndu, si->Shading.dndv, false);
}
} // namespace filianore