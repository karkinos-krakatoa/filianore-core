#ifndef _PRIMITIVE_INTERSECTORS_H
#define _PRIMITIVE_INTERSECTORS_H

#include <optional>
#include <vector>
#include "../core/ray.h"
#include "primitiveintersectionresult.h"

namespace filianore
{

    struct PrimitiveIntersector
    {

        PrimitiveIntersector(const Bvh &_bvh, const std::vector<std::shared_ptr<Primitive>> &_primitives, bool _anyHit)
            : bvh(_bvh), primitives(_primitives), anyHit(_anyHit)
        {
        }

        std::optional<PrimitiveIntersectionResult> Intersect(size_t index, const Ray &ray) const;

        std::pair<const std::shared_ptr<Primitive> &, size_t> PrimitiveAt(size_t index) const;

        const Bvh &bvh;
        std::vector<std::shared_ptr<Primitive>> primitives;
        bool anyHit;

        ~PrimitiveIntersector() {}
    };

}

#endif