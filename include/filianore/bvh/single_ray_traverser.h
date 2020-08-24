#ifndef BVH_SINGLE_RAY_TRAVERSAL_H
#define BVH_SINGLE_RAY_TRAVERSAL_H

#include <cassert>

#include "bvh.h"
#include "../core/ray.h"
#include "node_intersectors.h"
#include "bvh_utils.h"

namespace filianore
{

    /// Single ray traversal algorithm, using the provided ray-node intersector.
    template <typename Bvh, size_t StackSize = 64, typename NodeIntersector = FastNodeIntersector<Bvh>>
    class SingleRayTraverser
    {
    public:
        static constexpr size_t stack_size = StackSize;

    private:
        struct Stack
        {
            using Element = const typename Bvh::Node *;

            Element elements[stack_size];
            size_t size = 0;

            void Push(const Element &t)
            {
                assert(size < stack_size);
                elements[size++] = t;
            }

            Element Pop()
            {
                assert(!Empty());
                return elements[--size];
            }

            bool Empty() const { return size == 0; }
        };

        template <typename PrimitiveIntersector, typename Statistics>
        bvh__always_inline__
            std::optional<typename PrimitiveIntersector::Result> &
            IntersectLeaf(
                const typename Bvh::Node &node,
                Ray &ray,
                std::optional<typename PrimitiveIntersector::Result> &best_hit,
                PrimitiveIntersector &primitive_intersector,
                Statistics &statistics) const
        {
            assert(node.isLeaf);
            size_t begin = node.firstChildOrPrimitive;
            size_t end = begin + node.primitiveCount;
            statistics.intersections += end - begin;
            for (size_t i = begin; i < end; ++i)
            {
                if (auto hit = primitive_intersector.Intersect(i, ray))
                {
                    best_hit = hit;
                    if (primitive_intersector.anyHit)
                    {
                        return best_hit;
                    }

                    ray.tMax = hit->Distance();
                }
            }
            return best_hit;
        }

        template <typename PrimitiveIntersector, typename Statistics>
        bvh__always_inline__
            std::optional<typename PrimitiveIntersector::Result>
            Intersect(Ray ray, PrimitiveIntersector &primitive_intersector, Statistics &statistics) const
        {
            auto best_hit = std::optional<typename PrimitiveIntersector::Result>(std::nullopt);

            // If the root is a leaf, intersect it and return
            if (bvh__unlikely(bvh.nodes[0].isLeaf))
            {
                return IntersectLeaf(bvh.nodes[0], ray, best_hit, primitive_intersector, statistics);
            }

            NodeIntersector node_intersector(ray);

            // This traversal loop is eager, because it immediately processes leaves instead of pushing them on the stack.
            // This is generally beneficial for performance because intersections will likely be found which will
            // allow to cull more subtrees with the ray-box test of the traversal loop.
            Stack stack;
            const auto *node = bvh.nodes.get();
            while (true)
            {
                statistics.traversal_steps++;

                auto first_child = node->firstChildOrPrimitive;
                const auto *left_child = &bvh.nodes[first_child + 0];
                const auto *right_child = &bvh.nodes[first_child + 1];
                auto distance_left = node_intersector.Intersect(*left_child, ray);
                auto distance_right = node_intersector.Intersect(*right_child, ray);

                if (distance_left.first <= distance_left.second)
                {
                    if (bvh__unlikely(left_child->isLeaf))
                    {
                        if (IntersectLeaf(*left_child, ray, best_hit, primitive_intersector, statistics) &&
                            primitive_intersector.anyHit)
                            break;
                        left_child = nullptr;
                    }
                }
                else
                    left_child = nullptr;

                if (distance_right.first <= distance_right.second)
                {
                    if (bvh__unlikely(right_child->isLeaf))
                    {
                        if (IntersectLeaf(*right_child, ray, best_hit, primitive_intersector, statistics) &&
                            primitive_intersector.anyHit)
                            break;
                        right_child = nullptr;
                    }
                }
                else
                    right_child = nullptr;

                if (bvh__likely((left_child != NULL) ^ (right_child != NULL)))
                {
                    node = left_child != NULL ? left_child : right_child;
                }
                else if (bvh__unlikely((left_child != NULL) & (right_child != NULL)))
                {
                    if (distance_left.first > distance_right.first)
                        std::swap(left_child, right_child);
                    stack.Push(right_child);
                    node = left_child;
                }
                else
                {
                    if (stack.Empty())
                        break;
                    node = stack.Pop();
                }
            }

            return best_hit;
        }

        const Bvh &bvh;

    public:
        /// Statistics collected during traversal.
        struct Statistics
        {
            size_t traversal_steps = 0;
            size_t intersections = 0;
        };

        SingleRayTraverser(const Bvh &bvh)
            : bvh(bvh)
        {
        }

        /// Intersects the BVH with the given ray and intersector.
        template <typename PrimitiveIntersector>
        bvh__always_inline__
            std::optional<typename PrimitiveIntersector::Result>
            Traverse(const Ray &ray, PrimitiveIntersector &intersector) const
        {
            struct
            {
                struct Empty
                {
                    Empty &operator++(int) { return *this; }
                    Empty &operator++() { return *this; }
                    Empty &operator+=(size_t) { return *this; }
                } traversal_steps, intersections;
            } statistics;
            return Intersect(ray, intersector, statistics);
        }

        /// Intersects the BVH with the given ray and intersector.
        /// Record statistics on the number of traversal and intersection steps.
        template <typename PrimitiveIntersector>
        bvh__always_inline__
            std::optional<typename PrimitiveIntersector::Result>
            Traverse(const Ray &ray, PrimitiveIntersector &primitive_intersector, Statistics &statistics) const
        {
            return Intersect(ray, primitive_intersector, statistics);
        }
    };

} // namespace filianore

#endif