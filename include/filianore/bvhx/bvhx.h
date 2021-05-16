#ifndef _BVHX_H
#define _BVHX_H

#include <cassert>
#include "utils.h"
#include "../core/aabb.h"

namespace filianore
{

    struct Bvh
    {
        using IndexType = typename SizedIntegerType<sizeof(float) * CHAR_BIT>::Unsigned;

        // This should be in 32 bits
        struct BvhNode
        {
            float bounds[6];
            IndexType primitiveCount;
            IndexType firstChildOrPrimitive;

            bool IsLeaf() const
            {
                return primitiveCount != 0;
            }

            struct AABBProxy
            {
                BvhNode &node;

                AABBProxy(BvhNode &_node)
                    : node(_node) {}

                AABBProxy &operator=(const AABB &bbox)
                {
                    node.bounds[0] = bbox.pMin.params[0];
                    node.bounds[1] = bbox.pMax.params[0];
                    node.bounds[2] = bbox.pMin.params[1];
                    node.bounds[3] = bbox.pMax.params[1];
                    node.bounds[4] = bbox.pMin.params[2];
                    node.bounds[5] = bbox.pMax.params[2];
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

                AABBProxy &Extend(const AABB &_bbox)
                {
                    AABB bbox(ToBoundingBox());
                    bbox.Extend(_bbox);
                    return *this = bbox;
                }

                AABBProxy &Extend(const StaticArray<float, 3> &vector)
                {
                    AABB bbox(ToBoundingBox());
                    bbox.Extend(vector);
                    return *this = bbox;
                }
            };

            AABBProxy AABB_Proxy()
            {
                return AABBProxy(*this);
            }

            const AABBProxy AABB_Proxy() const
            {
                return AABBProxy(*const_cast<BvhNode *>(this));
            }
        };

        // Returns index of its sibling
        static size_t Sibling(size_t index)
        {
            assert(index != 0);
            return index % 2 == 1 ? index + 1 : index - 1;
        }

        // Return is node is a left sibling
        static bool IsLeftSibling(size_t index)
        {
            assert(index != 0);
            return index % 2 == 1;
        }

        std::unique_ptr<BvhNode[]> nodes;
        std::unique_ptr<size_t[]> primitiveIndices;
        size_t nodeCount = 0;
    };

}

#endif