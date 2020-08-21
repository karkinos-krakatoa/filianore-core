#ifndef BVH_PRIMITIVE_INTERSECTORS_H
#define BVH_PRIMITIVE_INTERSECTORS_H

#include <optional>

#include "../core/ray.h"
#include "../core/primitive.h"
#include "../core/interaction.h"

#include "bvh.h"

namespace filianore
{

    /// Base class for primitive intersectors.
    template <bool PreShuffled, bool AnyHit>
    struct PrimitiveIntersector
    {
        PrimitiveIntersector(const Bvh &bvh, const std::vector<std::shared_ptr<Primitive>> &primitives)
            : bvh(bvh), primitives(primitives)
        {
        }

        std::pair<std::shared_ptr<Primitive>, size_t> PrimitiveAt(size_t index) const
        {
            index = PreShuffled ? index : bvh.primitiveIndices[index];
            return std::pair<std::shared_ptr<Primitive>, size_t>{primitives[index], index};
        }

        const Bvh &bvh;
        std::vector<std::shared_ptr<Primitive>> primitives;

        static constexpr bool anyHit = AnyHit;

    protected:
        ~PrimitiveIntersector() {}
    };

    /// An intersector that looks for the closest intersection.
    template <bool PreShuffled = false>
    struct ClosestPrimitiveIntersector : public PrimitiveIntersector<PreShuffled, false>
    {
        struct Result
        {
            size_t primitiveIndex;
            Interaction interaction;

            float Distance() const { return interaction.t; }
        };

        ClosestPrimitiveIntersector(const Bvh &bvh, const std::vector<std::shared_ptr<Primitive>> &primitives)
            : PrimitiveIntersector<PreShuffled, false>(bvh, primitives)
        {
        }

        std::optional<Result> Intersect(size_t index, const Ray &ray) const
        {
            auto [p, i] = this->PrimitiveAt(index);

            Interaction isect;
            if (p->Intersect(ray, &isect))
            {
                return std::make_optional(Result{i, isect});
            }
            return std::nullopt;
        }
    };

    /// An intersector that exits after the first hit and only stores the distance to the primitive.
    template <bool PreShuffled = false>
    struct AnyPrimitiveIntersector : public PrimitiveIntersector<PreShuffled, true>
    {
        struct Result
        {
            float t;
            float Distance() const { return t; }
        };

        AnyPrimitiveIntersector(const Bvh &bvh, const std::vector<std::shared_ptr<Primitive>> &primitives)
            : PrimitiveIntersector<PreShuffled, true>(bvh, primitives)
        {
        }

        std::optional<Result> Intersect(size_t index, const Ray &ray) const
        {
            auto [p, i] = this->PrimitiveAt(index);

            Interaction isect;
            if (p->Intersect(ray, &isect))
            {
                return std::make_optional(Result{isect.t});
            }
            return std::nullopt;
        }
    };

} // namespace filianore

#endif