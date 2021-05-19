#include "filianore/bvhx/raytraverser.h"
#include "filianore/bvhx/primitiveintersectors.h"
#include "filianore/bvhx/nodeintersectors.h"

namespace filianore
{
    std::optional<PrimitiveIntersectionResult> &RayTraverser::IntersectLeaf(
        const Bvh::BvhNode &node,
        const Ray &ray,
        std::optional<PrimitiveIntersectionResult> &bestHit,
        PrimitiveIntersector &primitiveIntersector) const
    {
        assert(node.IsLeaf());
        size_t begin = node.firstChildOrPrimitive;
        size_t end = begin + node.primitiveCount;

        for (size_t i = begin; i < end; ++i)
        {
            std::optional<PrimitiveIntersectionResult> hit = primitiveIntersector.Intersect(i, ray);
            if (hit)
            {
                bestHit = hit;
                if (primitiveIntersector.anyHit)
                {
                    return bestHit;
                }
                ray.tMax = hit->Distance();
            }
        }
        return bestHit;
    }

    std::optional<PrimitiveIntersectionResult> RayTraverser::Intersect(
        const Ray &ray,
        PrimitiveIntersector &primitiveIntersector) const
    {
        auto bestHit = std::optional<PrimitiveIntersectionResult>(std::nullopt);

        if (Likely(bvh.nodes[0].IsLeaf()))
        {
            return IntersectLeaf(bvh.nodes[0], ray, bestHit, primitiveIntersector);
        }

        RobustNodeIntersector nodeIntersector(ray);

        Stack stack;
        Bvh::BvhNode *leftChild = &bvh.nodes[bvh.nodes[0].firstChildOrPrimitive];
        while (true)
        {
            Bvh::BvhNode *rightChild = leftChild + 1;

            auto distanceLeft = nodeIntersector.Intersect(*leftChild, ray);
            auto distanceRight = nodeIntersector.Intersect(*rightChild, ray);

            if (distanceLeft.first <= distanceLeft.second)
            {
                if (Likely(leftChild->IsLeaf()))
                {
                    auto iresult = IntersectLeaf(*leftChild, ray, bestHit, primitiveIntersector) && primitiveIntersector.anyHit;
                    if (iresult)
                    {
                        break;
                    }
                    leftChild = nullptr;
                }
            }
            else
            {
                leftChild = nullptr;
            }

            if (distanceRight.first <= distanceRight.second)
            {
                if (Likely(rightChild->IsLeaf()))
                {
                    auto iresult = IntersectLeaf(*rightChild, ray, bestHit, primitiveIntersector) && primitiveIntersector.anyHit;
                    if (iresult)
                    {
                        break;
                    }
                    rightChild = nullptr;
                }
            }
            else
            {
                rightChild = nullptr;
            }

            if (leftChild)
            {
                if (rightChild)
                {
                    if (distanceLeft.first > distanceRight.first)
                    {
                        std::swap(leftChild, rightChild);
                    }
                    stack.Push(rightChild->firstChildOrPrimitive);
                }
                leftChild = &bvh.nodes[leftChild->firstChildOrPrimitive];
            }
            else if (rightChild)
            {
                leftChild = &bvh.nodes[rightChild->firstChildOrPrimitive];
            }
            else
            {
                if (stack.Empty())
                {
                    break;
                }
                leftChild = &bvh.nodes[stack.Pop()];
            }
        }

        return bestHit;
    }

    std::optional<PrimitiveIntersectionResult> RayTraverser::Traverse(
        const Ray &ray, PrimitiveIntersector &intersector) const
    {
        return Intersect(ray, intersector);
    }
}