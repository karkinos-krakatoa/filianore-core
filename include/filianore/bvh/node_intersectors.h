#ifndef BVH_NODE_INTERSECTORS_H
#define BVH_NODE_INTERSECTORS_H

#include "../core/ray.h"
#include "bvh_platform.h"
#include "bvh_utils.h"

namespace filianore
{

    using Vec3 = StaticArray<float, 3>;

    /// Base class for ray-node intersection algorithms. Does ray octant classification.
    template <typename Bvh, typename Derived>
    struct NodeIntersector
    {
        std::array<int, 3> octant;

        NodeIntersector(const Ray &ray)
            : octant{
                  ray.dir.params[0] < float(0),
                  ray.dir.params[1] < float(0),
                  ray.dir.params[2] < float(0)}
        {
        }

        bvh__always_inline__ float IntersectAxis(int axis, const Vec3 &p, const Ray &ray) const
        {
            return static_cast<const Derived *>(this)->IntersectAxis(axis, p, ray);
        }

        bvh__always_inline__ std::pair<float, float> Intersect(const Bvh::Node &node, const Ray &ray) const
        {
            Vec3 entry, exit;
            entry.params[0] = IntersectAxis(0, node.bounds[0 * 2 + octant[0]], ray);
            entry.params[1] = IntersectAxis(1, node.bounds[1 * 2 + octant[1]], ray);
            entry.params[2] = IntersectAxis(2, node.bounds[2 * 2 + octant[2]], ray);
            exit.params[0] = IntersectAxis(0, node.bounds[0 * 2 + 1 - octant[0]], ray);
            exit.params[1] = IntersectAxis(1, node.bounds[1 * 2 + 1 - octant[1]], ray);
            exit.params[2] = IntersectAxis(2, node.bounds[2 * 2 + 1 - octant[2]], ray);
            // Note: This order for the min/max operations is guaranteed not to produce NaNs
            return std::make_pair(
                RobustMax(entry.params[0], RobustMax(entry.params[1], RobustMax(entry.params[2], ray.tMin))),
                RobustMin(exit.params[0], RobustMin(exit.params[1], RobustMin(exit.params[2], ray.tMax))));
        }

    protected:
        ~NodeIntersector() {}
    };

    /// Fully robust ray-node intersection algorithm (see "Robust BVH Ray Traversal", by T. Ize).
    template <typename Bvh>
    struct RobustNodeIntersector : public NodeIntersector<Bvh, RobustNodeIntersector<Bvh>>
    {
        // Padded inverse direction to avoid false-negatives in the ray-node test.
        Vec3 paddedInverseDirection;

        RobustNodeIntersector(const Ray &ray)
            : NodeIntersector<Bvh, RobustNodeIntersector<Bvh>>(ray)
        {
            Ray _ray = ray;
            auto inverseDirection = _ray.dir.Inverse();

            paddedInverseDirection = Vec3(
                AddUlpMagnitude(inverseDirection.params[0], 2),
                AddUlpMagnitude(inverseDirection.params[1], 2),
                AddUlpMagnitude(inverseDirection.params[2], 2));
        }

        bvh__always_inline__ float IntersectAxis(int axis, const Vec3 &p, const Ray &ray) const
        {
            return (p.params[axis] - ray.origin.params[axis]) * paddedInverseDirection.params[axis];
        }

        using NodeIntersector<Bvh, RobustNodeIntersector<Bvh>>::Intersect;
    };

    /// Semi-robust, fast ray-node intersection algorithm.
    template <typename Bvh>
    struct FastNodeIntersector : public NodeIntersector<Bvh, FastNodeIntersector<Bvh>>
    {
        Vec3 scaledOrigin;
        Vec3 inverseDirection;

        FastNodeIntersector(const Ray &ray)
            : NodeIntersector<Bvh, FastNodeIntersector<Bvh>>(ray)
        {
            Ray _ray = ray;
            inverseDirection = _ray.dir.Inverse();
            scaledOrigin = _ray.origin.Neg() * inverseDirection;
        }

        bvh__always_inline__ float IntersectAxis(int axis, const Vec3 &p, const Ray &) const
        {
            return p.params[axis] * inverseDirection.params[axis] + scaledOrigin.params[axis];
        }

        using NodeIntersector<Bvh, FastNodeIntersector<Bvh>>::Intersect;
    };

} // namespace filianore

#endif