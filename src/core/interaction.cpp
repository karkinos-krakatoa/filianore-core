#include "filianore/core/interaction.h"
#include "filianore/core/primitive.h"
#include "filianore/core/illuminant.h"
#include "filianore/maths/vec3_math.h"

namespace filianore
{

    PrincipalSpectrum SurfaceInteraction::Le(const StaticArray<float, 3> &w) const
    {
        const AreaIlluminant *areaIllum = primitive->GetAreaIlluminant();
        return areaIllum ? areaIllum->L(*this, w) : PrincipalSpectrum(0.f);
    }

    void SurfaceInteraction::ComputeScatteringFunctions(const Ray &ray)
    {
        if (primitive)
        {
            primitive->ComputeScatteringFunctions(this);
        }
    }

    void SurfaceInteraction::SetShadingGeometry(const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                                                const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv,
                                                bool orientationIsAuthoritative)
    {
        shading.n = Cross(_dpdu, _dpdv).Normalize();

        if (orientationIsAuthoritative)
        {
            n = Faceforward(n, shading.n);
        }
        else
        {
            shading.n = Faceforward(shading.n, n);
        }

        dpdu = _dpdu;
        dpdv = _dpdv;
        dndu = _dndu;
        dndv = _dndv;
    }

} // namespace filianore