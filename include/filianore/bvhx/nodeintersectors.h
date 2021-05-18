#ifndef _NODE_INTERSECTORS_H
#define _NODE_INTERSECTORS_H

#include <cmath>
#include "../core/aabb.h"
#include "utils.h"
#include "bvhx.h"

namespace filianore
{

    struct RobustNodeIntersector
    {
        std::array<int, 3> octant;
        StaticArray<float, 3> paddedInverseDirection;
        StaticArray<float, 3> inverseDirection;

        RobustNodeIntersector(const Ray &ray)
            : octant{
                  std::signbit(ray.dir.params[0]),
                  std::signbit(ray.dir.params[1]),
                  std::signbit(ray.dir.params[2])}
        {
            inverseDirection = ray.dir;
            inverseDirection = inverseDirection.Inverse();

            paddedInverseDirection = StaticArray<float, 3>(
                AddUlpMagnitude(inverseDirection.params[0], 2),
                AddUlpMagnitude(inverseDirection.params[1], 2),
                AddUlpMagnitude(inverseDirection.params[2], 2));
        }

        FILIANORE_INLINE float IntersectAxis(int axis, float p, const Ray &ray, bool isMin) const
        {
            return (p - ray.origin.params[axis]) * (isMin ? inverseDirection.params[axis] : paddedInverseDirection.params[axis]);
        }

        FILIANORE_INLINE std::pair<float, float> Intersect(const Bvh::BvhNode &node, const Ray &ray) const
        {
            StaticArray<float, 3> entry, exit;
            entry.params[0] = IntersectAxis(0, node.bounds[0 * 2 + octant[0]], ray, true);
            entry.params[1] = IntersectAxis(1, node.bounds[1 * 2 + octant[1]], ray, true);
            entry.params[2] = IntersectAxis(2, node.bounds[2 * 2 + octant[2]], ray, true);

            exit.params[0] = IntersectAxis(0, node.bounds[0 * 2 + 1 - octant[0]], ray, false);
            exit.params[1] = IntersectAxis(1, node.bounds[1 * 2 + 1 - octant[1]], ray, false);
            exit.params[2] = IntersectAxis(2, node.bounds[2 * 2 + 1 - octant[2]], ray, false);

            return std::make_pair(
                RobustMax(RobustMax(entry.params[0], entry.params[1]), RobustMax(entry.params[2], ray.tMin)),
                RobustMin(RobustMin(exit.params[0], exit.params[1]), RobustMin(exit.params[2], ray.tMax)));
        }

        ~RobustNodeIntersector() {}
    };
}

#endif