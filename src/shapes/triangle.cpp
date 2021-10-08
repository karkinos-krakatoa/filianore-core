#include "filianore/shapes/triangle.h"
#include "filianore/core/interaction.h"

namespace filianore
{

    AABB Triangle::WorldBound() const
    {
        AABB box(v1.vertex);
        box.Extend(v2.vertex);
        box.Extend(v3.vertex);
        return box;
    }

    bool Triangle::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        StaticArray<float, 3> e1 = v2.vertex - v1.vertex;
        StaticArray<float, 3> e2 = v3.vertex - v1.vertex;

        StaticArray<float, 3> n = Cross(e1, e2);

        float det = -Dot(ray.dir, n);

        StaticArray<float, 3> ao = ray.origin - v1.vertex;
        StaticArray<float, 3> dao = Cross(ao, ray.dir);

        float invDet = 1.f / det;
        float alpha = Dot(e2, dao) * invDet;
        float beta = -Dot(e1, dao) * invDet;
        float t = Dot(ao, n) * invDet;

        if ((t < ray.tMax && t > ray.tMin) && alpha > 0 && beta > 0 && (alpha + beta) < 1.f)
        {
            StaticArray<float, 3> revRay = ray.dir;

            // Shading params
            StaticArray<float, 3> dpdu, dpdv;

            StaticArray<float, 2> duv02 = v1.uv - v3.uv;
            StaticArray<float, 2> duv12 = v2.uv - v3.uv;
            StaticArray<float, 3> dp02 = v1.vertex - v3.vertex;
            StaticArray<float, 3> dp12 = v2.vertex - v3.vertex;

            float determinant = duv02.x() * duv12.y() - duv02.y() * duv12.x();
            bool denegerateUV = std::abs(determinant) < 1e-8;
            if (!denegerateUV)
            {
                float invDet = 1.f / determinant;
                dpdu = (dp02 * duv12.y() - dp12 * duv02.y()) * invDet;
                dpdv = (dp02 * -duv12.x() + dp12 * duv02.x()) * invDet;
            }
            if (denegerateUV || Cross(dpdu, dpdv).LengthSquared() == 0)
            {
                if (n.LengthSquared() == 0)
                    return false;

                CoordinateSystem(n, &dpdu, &dpdv);
            }

            *isect = SurfaceInteraction(t, StaticArray<float, 3>(), StaticArray<float, 2>(),
                                        dpdu, dpdv, StaticArray<float, 3>(), StaticArray<float, 3>(),
                                        revRay.Neg(), this, 0);

            isect->p = ray.PointAtT(t);

            // Further Evaluations
            const StaticArray<float, 3> f1 = v1.vertex - isect->p;
            const StaticArray<float, 3> f2 = v2.vertex - isect->p;
            const StaticArray<float, 3> f3 = v3.vertex - isect->p;

            const float a = Cross((v1.vertex - v2.vertex), (v1.vertex - v3.vertex)).Length();
            const float invA = 1.f / a;
            const float a1 = Cross(f2, f3).Length() * invA;
            const float a2 = Cross(f3, f1).Length() * invA;
            const float a3 = Cross(f1, f2).Length() * invA;

            isect->uv = v1.uv * a1 + v2.uv * a2 + v3.uv * a3;
            isect->pError = Abs(isect->p) * Gamma<float>(5);

            isect->n = isect->Shading.n = Cross(dp02, dp12).Normalize();

            // if (allNormalsInMesh)
            // {
            //     // Interpolated Normal
            //     StaticArray<float, 3> ns = (v1.normal * a1 + v2.normal * a2 + v3.normal * a3);
            //     if (ns.LengthSquared() > 0)
            //         ns = ns.Normalize();
            //     else
            //         ns = isect->n;

            //     // Shading tangent
            //     StaticArray<float, 3> ss = isect->dpdu.Normalize();

            //     // Shading Bitangent
            //     StaticArray<float, 3> ts = Cross(ss, ns);
            //     if (ts.LengthSquared() > 0.f)
            //     {
            //         ts = ts.Normalize();
            //         ss = Cross(ts, ns);
            //     }
            //     else
            //     {
            //         CoordinateSystem(ns, &ss, &ts);
            //     }

            //     StaticArray<float, 3> dndu, dndv;
            //     StaticArray<float, 3> dn1 = v1.normal - v3.normal;
            //     StaticArray<float, 3> dn2 = v2.normal - v3.normal;
            //     if (denegerateUV)
            //     {
            //         StaticArray<float, 3> dn = Cross(v3.normal - v1.normal, v2.normal - v1.normal);
            //         if (dn.LengthSquared() != 0)
            //         {
            //             CoordinateSystem(dn, &dndu, &dndv);
            //         }
            //     }
            //     else
            //     {
            //         dndu = (dn1 * duv12.y() - dn2 * duv02.y()) * invDet;
            //         dndv = (dn1 * -duv12.x() + dn2 * duv02.x()) * invDet;
            //     }

            //     isect->SetShadingGeometry(ss, ts, dndu, dndv, true);
            // }

            return true;
        }

        return false;
    }

    StaticArray<float, 3> Triangle::Centroid() const
    {
        return (v1.vertex + v2.vertex + v3.vertex) / 3.f;
    }

    float Triangle::Area() const
    {
        return 0.5f * Cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).Length();
    }

    Interaction Triangle::Sample(const StaticArray<float, 2> &u, float *pdf) const
    {
        const StaticArray<float, 3> p0 = v1.vertex;
        const StaticArray<float, 3> p1 = v2.vertex;
        const StaticArray<float, 3> p2 = v3.vertex;

        float uu = std::sqrt(u.x());
        float u1 = 1.f - uu;
        float u2 = u.y() * uu;

        Interaction it;

        it.p = p0 * u1 + p1 * u2 + p2 * (1.f - u1 - u2);
        it.n = Cross(p1 - p0, p2 - p0).Normalize();

        StaticArray<float, 3> pAbsSum = Abs(p0 * u1) + Abs(p1 * u2) + Abs(p2 * (1 - u1 - u2));
        it.pError = pAbsSum * Gamma<float>(6);

        *pdf = 1.f / Area();

        return it;
    }

} // namespace filianore