#ifndef _BVH_H
#define _BVH_H

#include <climits>
#include <memory>
#include <cassert>

#include "filianore/core/aabb.h"
#include "filianore/core/util.h"

namespace filianore
{
    struct Bvh
    {
        using IndexType = typename SizedIntegerType<sizeof(float) * CHAR_BIT>::Unsigned;

        struct Node
        {
            float bounds[6];
            bool isLeaf : 1;
            IndexType primitiveCount : sizeof(IndexType) * CHAR_BIT - 1;
            IndexType firstChildOrPrimitive;

            struct AABBProxy
            {
                Node &node;

                AABBProxy(Node &node)
                    : node(node)
                {
                }

                AABBProxy &operator=(const AABB &bbox)
                {
                    node.bounds[0] = bbox.pMin.params[0];
                    node.bounds[1] = bbox.pMax.params[0];
                    node.bounds[2] = bbox.pMin.params[1];
                    node.bounds[3] = bbox.pMin.params[1];
                    node.bounds[4] = bbox.pMin.params[2];
                    node.bounds[5] = bbox.pMin.params[2];
                    return *this;
                }

                operator AABB() const
                {
                    return AABB(
                        StaticArray<float, 3>(node.bounds[0], node.bounds[2], node.bounds[4]),
                        StaticArray<float, 3>(node.bounds[1], node.bounds[3], node.bounds[5]));
                }

                AABB ToBoundingBox() const
                {
                    return static_cast<AABB>(*this);
                }

                float HalfArea() const
                {
                    return ToBoundingBox().HalfArea();
                }

                AABBProxy &Extend(const AABB &bbox)
                {
                    AABB eBox(ToBoundingBox());
                    eBox.Extend(bbox);
                    return *this = eBox;
                }

                AABBProxy &Extend(const StaticArray<float, 3> &p)
                {
                    AABB eBox(ToBoundingBox());
                    eBox.Extend(p);
                    return *this = eBox;
                }
            };

            AABBProxy AABB_Proxy()
            {
                return AABBProxy(*this);
            }

            const AABBProxy AABB_Proxy() const
            {
                return AABBProxy(*const_cast<Node *>(this));
            }
        };

        static size_t Sibling(size_t index)
        {
            assert(index != 0);
            return index % 2 == 1 ? index + 1 : index - 1;
        }

        static bool IsLeftSibling(size_t index)
        {
            assert(index != 0);
            return index % 2 == 1;
        }

        std::unique_ptr<Node[]> nodes;
        std::unique_ptr<size_t[]> primitiveIndices;

        size_t nodeCount = 0;
    };

} // namespace filianore

#endif