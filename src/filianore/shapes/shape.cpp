#include "shape.h"
#include "../core/interaction.h"

namespace filianore {
bool Shape::intersect_p(const Ray &ray) const {
    return intersect(ray, 0);
}

float Shape::pdf(const Interaction &isect) const {
    return 1.f / area();
}

float Shape::pdf(const Interaction &isect, const Vector3f &wi) const {
    Ray ray = isect.kindle_ray(wi);
    SurfaceInteraction isc;
    if (!intersect(ray, &isc)) {
        return 0.f;
    }

    Vector3f wiNeg = wi;
    float pdf = (isect.p - isc.p).length_squared() / (abs_dot(isc.n, -wiNeg) * area());
    if (std::isinf(pdf)) {
        pdf = 0.f;
    }
    return pdf;
}

Interaction Shape::sample(const Interaction &isect, const Vector2f &u, float *pdf) const {
    Interaction isc = sample(u, pdf);
    Vector3f wi = isc.p - isect.p;
    if (wi.length_squared() == 0) {
        *pdf = 0.f;
    } else {
        wi = normalize(wi);
        *pdf *= (isect.p - isc.p).length_squared() / abs_dot(isc.n, -wi);
        if (std::isinf(*pdf)) {
            *pdf = 0.f;
        }
    }
    return isc;
}

} // namespace filianore