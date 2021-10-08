#include "filianore/core/material.h"
#include "filianore/textures/imagemap.h"
#include "filianore/core/primitive.h"
#include "filianore/core/interaction.h"

namespace filianore
{

    void Material::BumpEvaluate(const std::shared_ptr<Texture<float>> &bumpMap, SurfaceInteraction *si)
    {
        SurfaceInteraction siEval = *si;

        float du = 0.0005f, dv = 0.0005f;

        // Shift si in u-direction
        siEval.p = si->p + si->dpdu * du;
        siEval.uv = si->uv + StaticArray<float, 2>(du, 0.f);
        siEval.n = Cross(si->Shading.dpdu, si->Shading.dpdv).Normalize();
        float uDisplace = bumpMap->Evaluate(siEval);

        // Shift si in v-direction
        siEval.p = si->p + si->dpdv * dv;
        siEval.uv = si->uv + StaticArray<float, 2>(0.f, dv);
        siEval.n = Cross(si->Shading.dpdu, si->Shading.dpdv).Normalize();
        float vDisplace = bumpMap->Evaluate(siEval);

        float displace = bumpMap->Evaluate(*si);

        StaticArray<float, 3> dpdu = si->Shading.dpdu + si->Shading.n * ((uDisplace - displace) / du) * 4.f;
        StaticArray<float, 3> dpdv = si->Shading.dpdv + si->Shading.n * ((vDisplace - displace) / dv) * 4.f;

        si->SetShadingGeometry(dpdu, dpdv, si->Shading.dndu, si->Shading.dndv, false);
    }
}