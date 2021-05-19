#ifndef _RAY_TRAVERSER_H
#define _RAY_TRAVERSER_H

#include <optional>
#include <cassert>
#include "bvhx.h"
#include "../core/elemental.h"
#include "primitiveintersectionresult.h"

namespace filianore
{

    class RayTraverser
    {
    public:
        static constexpr size_t stackSize = 64;

    private:
        const Bvh &bvh;

        struct Stack
        {
            using Element = Bvh::IndexType;

            Element elements[stackSize];
            size_t size = 0;

            void Push(const Element &e)
            {
                assert(size < stackSize);
                elements[size++] = e;
            }

            Element Pop()
            {
                assert(!Empty());
                return elements[--size];
            }

            bool Empty() const
            {
                return size == 0;
            }
        };

        std::optional<PrimitiveIntersectionResult> &IntersectLeaf(
            const Bvh::BvhNode &node,
            const Ray &ray,
            std::optional<PrimitiveIntersectionResult> &bestHit,
            PrimitiveIntersector &primitiveIntersector) const;

        std::optional<PrimitiveIntersectionResult> Intersect(
            const Ray &ray,
            PrimitiveIntersector &primitiveIntersector) const;

    public:
        RayTraverser(const Bvh &_bvh)
            : bvh(_bvh)
        {
        }

        std::optional<PrimitiveIntersectionResult> Traverse(
            const Ray &ray, PrimitiveIntersector &intersector) const;
    };
}

#endif