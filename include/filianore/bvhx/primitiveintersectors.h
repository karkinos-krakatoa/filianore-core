#ifndef _PRIMITIVE_INTERSECTORS_H
#define _PRIMITIVE_INTERSECTORS_H

#include <optional>
#include "../core/ray.h"
#include "bvhx.h"
#include "../core/primitive.h"
#include "../core/interaction.h"

namespace filianore
{

    struct PrimitiveIntersector
    {
        PrimitiveIntersector(const Bvh &_bvh, const std::vector<std::shared_ptr<Primitive>> &_primitives)
            : bvh(_bvh), primitives(_primitives)
        {
        }

        std::pair<const std::shared_ptr<Primitive> &, size_t> PrimitiveAt(size_t index) const
        {
            index = bvh.primitiveIndices[index];
            return std::pair<const std::shared_ptr<Primitive> &, size_t>{primitives[index], index};
        }

        const Bvh &bvh;
        std::vector<std::shared_ptr<Primitive>> primitives;

    protected:
        ~PrimitiveIntersector()
        {
        }
    };

    struct ClosestPrimitiveIntersector : public PrimitiveIntersector
    {
        struct Result
        {
            size_t primitiveIndex;
            SurfaceInteraction isect;

            float Distance() const { return isect.t; }
        };

        ClosestPrimitiveIntersector(const Bvh &_bvh, const std::vector<std::shared_ptr<Primitive>> &_primitives)
            : PrimitiveIntersector(_bvh, _primitives)
        {
        }

        std::optional<Result> Intersect(size_t index, const Ray &ray) const
        {
            auto [prim, idx] = this->PrimitiveAt(index);

            SurfaceInteraction isect;
            bool hit = prim->Intersect(ray, &isect);
            if (hit)
            {
                return std::make_optional(Result{idx, isect});
            }

            return std::nullopt;
        }
    };
}

#endif