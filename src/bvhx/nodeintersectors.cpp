#include "filianore/bvhx/nodeintersectors.h"

namespace filianore
{

    float RobustNodeIntersector::IntersectAxis(int axis, float p, const Ray &ray, bool isMin) const
    {
        return (p - ray.origin.params[axis]) * (isMin ? inverseDirection.params[axis] : inverseDirection.params[axis]);
    }

    std::pair<float, float> RobustNodeIntersector::Intersect(const Bvh::BvhNode &node, const Ray &ray) const
    {
        StaticArray<float, 3> entry, exit;
        entry.params[0] = IntersectAxis(0, node.bounds[0 * 2 + octant[0]], ray, true);
        entry.params[1] = IntersectAxis(1, node.bounds[1 * 2 + octant[1]], ray, true);
        entry.params[2] = IntersectAxis(2, node.bounds[2 * 2 + octant[2]], ray, true);

        exit.params[0] = IntersectAxis(0, node.bounds[0 * 2 + 1 - octant[0]], ray, false);
        exit.params[1] = IntersectAxis(1, node.bounds[1 * 2 + 1 - octant[1]], ray, false);
        exit.params[2] = IntersectAxis(2, node.bounds[2 * 2 + 1 - octant[2]], ray, false);

        return std::make_pair(
            RobustMax(entry.params[0], RobustMax(entry.params[1], RobustMax(entry.params[2], ray.tMin))),
            RobustMin(exit.params[0], RobustMin(exit.params[1], RobustMin(exit.params[2], ray.tMax))));
    }

}