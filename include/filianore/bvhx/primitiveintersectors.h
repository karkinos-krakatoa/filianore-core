#ifndef _PRIMITIVE_INTERSECTORS_H
#define _PRIMITIVE_INTERSECTORS_H

#include <optional>
#include "../core/ray.h"

namespace filianore
{

    template <typename Primitive, typename SurfaceInteraction>
    struct PrimitiveIntersector
    {
        struct PrimitiveIntersectionResult
        {
            size_t primitiveIndex;
            SurfaceInteraction isect;

            float Distance() const { return isect.t; }
        };

        PrimitiveIntersector(const Bvh &_bvh, const std::vector<std::shared_ptr<Primitive>> &_primitives, bool _anyHit)
            : bvh(_bvh), primitives(_primitives), anyHit(_anyHit)
        {
        }

        std::optional<PrimitiveIntersectionResult> Intersect(size_t index, const Ray &ray) const
        {
            auto [prim, idx] = this->PrimitiveAt(index);

            SurfaceInteraction isect;
            bool hit = prim->Intersect(ray, &isect);
            if (hit)
            {
                return std::make_optional(PrimitiveIntersectionResult{idx, isect});
            }

            return std::nullopt;
        }

        std::pair<const std::shared_ptr<Primitive> &, size_t> PrimitiveAt(size_t index) const
        {
            index = bvh.primitiveIndices[index];
            return std::pair<const std::shared_ptr<Primitive> &, size_t>{primitives[index], index};
        }

        const Bvh &bvh;
        std::vector<std::shared_ptr<Primitive>> primitives;
        bool anyHit;

        ~PrimitiveIntersector() {}
    };

}

#endif