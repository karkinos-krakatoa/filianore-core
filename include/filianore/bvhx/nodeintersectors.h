#ifndef _NODE_INTERSECTORS_H
#define _NODE_INTERSECTORS_H

#include <cmath>
#include <array>
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

        float IntersectAxis(int axis, float p, const Ray &ray, bool isMin) const;

        std::pair<float, float> Intersect(const Bvh::BvhNode &node, const Ray &ray) const;

        ~RobustNodeIntersector() {}
    };
}

#endif