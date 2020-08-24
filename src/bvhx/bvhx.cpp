#include "filianore/bvhx/bvhx.h"
#include "filianore/core/interaction.h"

namespace filianore
{

    bool BVH::IntersectMain(const Ray &_ray, SurfaceInteraction *isect, bool occlusion) const
    {
        Ray ray = _ray;
        *isect = SurfaceInteraction(0, StaticArray<float, 3>(), StaticArray<float, 3>(), StaticArray<float, 2>(), ray.dir.Neg(), NULL, 0);

        isect->t = Infinity<float>();
        isect->shape = NULL;
        float bbhits[4];
        int32_t closer, other;

        BVHTraversal todo[64];
        int32_t stackptr = 0;

        todo[stackptr].i = 0;
        todo[stackptr].mint = -Infinity<float>();

        while (stackptr >= 0)
        {
            int ni = todo[stackptr].i;
            float near = todo[stackptr].mint;
            stackptr--;
            const BVHFlatNode &node(flatTree[ni]);

            if (near > isect->t)
            {
                continue;
            }

            if (node.rightOffset == 0)
            {
                for (uint32_t o = 0; o < node.nPrims; ++o)
                {
                    SurfaceInteraction current;

                    const std::shared_ptr<Primitive> obj = (buildPrims)[node.start + o];
                    bool hit = obj->Intersect(ray, &current);

                    if (hit)
                    {
                        if (occlusion)
                        {
                            return true;
                        }

                        if (current.t >= 0 && current.t < isect->t)
                        {
                            *isect = current;
                        }
                    }
                }
            }
            else
            {
                bool hitc0 = flatTree[ni + 1].bbox.Intersect(ray, bbhits, bbhits + 1);
                bool hitc1 = flatTree[ni + node.rightOffset].bbox.Intersect(ray, bbhits + 2, bbhits + 3);

                if (hitc0 && hitc1)
                {
                    closer = ni + 1;
                    other = ni + node.rightOffset;

                    if (bbhits[2] < bbhits[0])
                    {
                        std::swap(bbhits[0], bbhits[2]);
                        std::swap(bbhits[1], bbhits[3]);
                        std::swap(closer, other);
                    }

                    todo[++stackptr] = BVHTraversal(other, bbhits[2]);
                    todo[++stackptr] = BVHTraversal(closer, bbhits[0]);
                }

                else if (hitc0)
                {
                    todo[++stackptr] = BVHTraversal(ni + 1, bbhits[0]);
                }

                else if (hitc1)
                {
                    todo[++stackptr] = BVHTraversal(ni + node.rightOffset, bbhits[2]);
                }
            }
        }

        if (isect->shape != NULL)
        {
            isect->p = ray.origin + ray.dir * isect->t;
        }

        return isect->shape != NULL;
    }

    void BVH::Build()
    {
        BVHBuildEntry todo[128];
        uint32_t stackptr = 0;
        const uint32_t Untouched = 0xffffffff;
        const uint32_t TouchedTwice = 0xfffffffd;

        todo[stackptr].start = 0;
        todo[stackptr].end = static_cast<uint32_t>(buildPrims.size());
        todo[stackptr].parent = 0xfffffffc;
        stackptr++;

        BVHFlatNode node;
        std::vector<BVHFlatNode> buildnodes;
        buildnodes.reserve(buildPrims.size());

        while (stackptr > 0)
        {
            BVHBuildEntry &bnode(todo[--stackptr]);
            uint32_t start = bnode.start;
            uint32_t end = bnode.end;
            uint32_t nPrims = end - start;

            nNodes++;
            node.start = start;
            node.nPrims = nPrims;
            node.rightOffset = Untouched;

            AABB bb((buildPrims)[start]->WorldBound());
            AABB bc((buildPrims)[start]->Centroid());
            for (uint32_t p = start + 1; p < end; ++p)
            {
                bb.Extend((buildPrims)[p]->WorldBound());
                bc.Extend((buildPrims)[p]->Centroid());
            }
            node.bbox = bb;

            if (nPrims <= leafSize)
            {
                node.rightOffset = 0;
                nLeafs++;
            }

            buildnodes.push_back(node);

            if (bnode.parent != 0xfffffffc)
            {
                buildnodes[bnode.parent].rightOffset--;

                // When this is the second touch, this is the right child.
                // The right child sets up the offset for the flat tree.
                if (buildnodes[bnode.parent].rightOffset == TouchedTwice)
                {
                    buildnodes[bnode.parent].rightOffset = nNodes - 1 - bnode.parent;
                }
            }

            if (node.rightOffset == 0)
                continue;

            uint32_t split_dim = bc.LargestAxis();

            float split_coord = .5f * (bc.pMin.params[split_dim] + bc.pMax.params[split_dim]);

            uint32_t mid = start;
            for (uint32_t i = start; i < end; ++i)
            {
                if ((buildPrims)[i]->Centroid().params[split_dim] < split_coord)
                {
                    std::swap((buildPrims)[i], (buildPrims)[mid]);
                    ++mid;
                }
            }

            if (mid == start || mid == end)
            {
                mid = start + (end - start) / 2;
            }

            // Push right child
            todo[stackptr].start = mid;
            todo[stackptr].end = end;
            todo[stackptr].parent = nNodes - 1;
            stackptr++;

            // Push left child
            todo[stackptr].start = start;
            todo[stackptr].end = mid;
            todo[stackptr].parent = nNodes - 1;
            stackptr++;
        }

        flatTree = new BVHFlatNode[nNodes];
        for (uint32_t n = 0; n < nNodes; ++n)
        {
            flatTree[n] = buildnodes[n];
        }
    }

    AABB BVH::WorldBound() const
    {
        AABB bb;
        for (auto prim : buildPrims)
        {
            bb.Extend(prim->WorldBound());
        }

        return bb;
    }

    StaticArray<float, 3> BVH::Centroid() const
    {
        // TO-DO
        return StaticArray<float, 3>();
    }

    bool BVH::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        return IntersectMain(ray, isect, false);
    }

    bool BVH::IntersectP(const Ray &ray) const
    {
        return IntersectMain(ray, nullptr, true);
    }

} // namespace filianore
