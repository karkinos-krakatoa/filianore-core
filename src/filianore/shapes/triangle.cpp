#include "triangle.h"
#include "../core/interaction.h"

namespace filianore {

AABB Triangle::world_bound() const {
    AABB box(v1.vertex);
    box.extend(v2.vertex);
    box.extend(v3.vertex);
    return box;
}

bool Triangle::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    Vector3f e1 = v2.vertex - v1.vertex;
    Vector3f e2 = v3.vertex - v1.vertex;

    Vector3f n = cross(e1, e2);

    float det = -dot(ray.dir, n);

    Vector3f ao = ray.origin - v1.vertex;
    Vector3f dao = cross(ao, ray.dir);

    float invDet = 1.f / det;
    float alpha = dot(e2, dao) * invDet;
    float beta = -dot(e1, dao) * invDet;
    float t = dot(ao, n) * invDet;

    if ((t < ray.tMax && t > ray.tMin) && alpha > 0 && beta > 0 && (alpha + beta) < 1.f) {
        Vector3f revRay = ray.dir;

        // Shading params
        Vector3f dpdu, dpdv;

        Vector2f duv02 = v1.uv - v3.uv;
        Vector2f duv12 = v2.uv - v3.uv;
        Vector3f dp02 = v1.vertex - v3.vertex;
        Vector3f dp12 = v2.vertex - v3.vertex;

        float determinant = duv02.x * duv12.y - duv02.y * duv12.x;
        bool denegerateUV = std::abs(determinant) < 1e-8;
        if (!denegerateUV) {
            float invDet = 1.f / determinant;
            dpdu = (dp02 * duv12.y - dp12 * duv02.y) * invDet;
            dpdv = (dp02 * -duv12.x + dp12 * duv02.x) * invDet;
        }
        if (denegerateUV || cross(dpdu, dpdv).length_squared() == 0) {
            if (n.length_squared() == 0)
                return false;

            coordinate_system(n, &dpdu, &dpdv);
        }

        *isect = SurfaceInteraction(t, Vector3f(), Vector2f(),
                                    dpdu, dpdv, Vector3f(), Vector3f(),
                                    -revRay, this, 0);

        isect->p = ray.point_at_t(t);

        // Further Evaluations
        const Vector3f f1 = v1.vertex - isect->p;
        const Vector3f f2 = v2.vertex - isect->p;
        const Vector3f f3 = v3.vertex - isect->p;

        const float a = cross((v1.vertex - v2.vertex), (v1.vertex - v3.vertex)).length();
        const float invA = 1.f / a;
        const float a1 = cross(f2, f3).length() * invA;
        const float a2 = cross(f3, f1).length() * invA;
        const float a3 = cross(f1, f2).length() * invA;

        isect->uv = v1.uv * a1 + v2.uv * a2 + v3.uv * a3;
        isect->pError = abs(isect->p) * gamma<float>(5);

        isect->n = isect->Shading.n = normalize(cross(dp02, dp12));

        // if (allNormalsInMesh)
        // {
        //     // Interpolated Normal
        //     Vector3f ns = (v1.normal * a1 + v2.normal * a2 + v3.normal * a3);
        //     if (ns.length_squared() > 0)
        //         ns = ns.Normalize();
        //     else
        //         ns = isect->n;

        //     // Shading tangent
        //     Vector3f ss = isect->dpdu.Normalize();

        //     // Shading Bitangent
        //     Vector3f ts = cross(ss, ns);
        //     if (ts.length_squared() > 0.f)
        //     {
        //         ts = ts.Normalize();
        //         ss = cross(ts, ns);
        //     }
        //     else
        //     {
        //         CoordinateSystem(ns, &ss, &ts);
        //     }

        //     Vector3f dndu, dndv;
        //     Vector3f dn1 = v1.normal - v3.normal;
        //     Vector3f dn2 = v2.normal - v3.normal;
        //     if (denegerateUV)
        //     {
        //         Vector3f dn = cross(v3.normal - v1.normal, v2.normal - v1.normal);
        //         if (dn.length_squared() != 0)
        //         {
        //             CoordinateSystem(dn, &dndu, &dndv);
        //         }
        //     }
        //     else
        //     {
        //         dndu = (dn1 * duv12.y - dn2 * duv02.y) * invDet;
        //         dndv = (dn1 * -duv12.x + dn2 * duv02.x) * invDet;
        //     }

        //     isect->set_shading_geometry(ss, ts, dndu, dndv, true);
        // }

        return true;
    }

    return false;
}

Vector3f Triangle::centroid() const {
    return (v1.vertex + v2.vertex + v3.vertex) / 3.f;
}

float Triangle::area() const {
    return 0.5f * cross((v2.vertex - v1.vertex), (v3.vertex - v1.vertex)).length();
}

Interaction Triangle::sample(const Vector2f &u, float *pdf) const {
    const Vector3f p0 = v1.vertex;
    const Vector3f p1 = v2.vertex;
    const Vector3f p2 = v3.vertex;

    float uu = std::sqrt(u.x);
    float u1 = 1.f - uu;
    float u2 = u.y * uu;

    Interaction it;

    it.p = p0 * u1 + p1 * u2 + p2 * (1.f - u1 - u2);
    it.n = normalize(cross(p1 - p0, p2 - p0));

    Vector3f pabsSum = abs(p0 * u1) + abs(p1 * u2) + abs(p2 * (1 - u1 - u2));
    it.pError = pabsSum * gamma<float>(6);

    *pdf = 1.f / area();

    return it;
}

} // namespace filianore