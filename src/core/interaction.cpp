#include "filianore/core/interaction.h"
#include "filianore/core/primitive.h"
#include "filianore/core/illuminant.h"
#include "filianore/maths/vec3_math.h"

namespace filianore
{

    SurfaceInteraction::SurfaceInteraction(float _t, const StaticArray<float, 3> &_p, const StaticArray<float, 2> &_uv,
                                           const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                                           const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv,
                                           const StaticArray<float, 3> &_wo, const Shape *_shape, float _time)
        : Interaction(_t, _p, Cross(_dpdu, _dpdv).Normalize(), _wo, _time), uv(_uv), shape(_shape),
          dpdu(_dpdu), dpdv(_dpdv), dndu(_dndu), dndv(_dndv)
    {
        Shading.n = n;
        Shading.dpdu = _dpdu;
        Shading.dpdv = _dpdv;
        Shading.dndu = _dndu;
        Shading.dndv = _dndv;
    }

    void SurfaceInteraction::SetShadingGeometry(const StaticArray<float, 3> &_dpdu, const StaticArray<float, 3> &_dpdv,
                                                const StaticArray<float, 3> &_dndu, const StaticArray<float, 3> &_dndv, bool orientationIsAuthoritative)
    {
        Shading.n = Cross(_dpdu, _dpdv).Normalize();

        if (orientationIsAuthoritative)
            n = Faceforward(n, Shading.n);
        else
            Shading.n = Faceforward(Shading.n, n);

        Shading.dpdu = _dpdu;
        Shading.dpdv = _dpdv;
        Shading.dndu = _dndu;
        Shading.dndv = _dndv;
    }

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

} // namespace filianore