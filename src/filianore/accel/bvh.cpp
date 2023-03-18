#include "bvh.h"
#include "../core/interaction.h"

namespace filianore {

bool BVH::intersect_main(const Ray &_ray, SurfaceInteraction *isect, bool occlusion) const {
    Ray ray = _ray;
    *isect = SurfaceInteraction(0, Vector3f(), Vector2f(),
                                Vector3f(), Vector3f(), Vector3f(), Vector3f(),
                                -ray.dir, NULL, 0);

    isect->t = INFINITI;
    isect->shape = NULL;
    float bbhits[4];
    int32_t closer, other;

    BVHTraversal todo[64];
    int32_t stackptr = 0;

    todo[stackptr].i = 0;
    todo[stackptr].mint = -INFINITI;

    while (stackptr >= 0) {
        int ni = todo[stackptr].i;
        float near = todo[stackptr].mint;
        stackptr--;
        const BVHFlatNode &node(flatTree[ni]);

        if (near > isect->t) {
            continue;
        }

        if (node.rightOffset == 0) {
            for (uint32_t o = 0; o < node.nPrims; ++o) {
                SurfaceInteraction current;
                const std::shared_ptr<Primitive> obj = (buildPrims)[node.start + o];
                bool hit = obj->intersect(ray, &current);

                if (hit) {
                    if (occlusion) {
                        return true;
                    }

                    if (current.t >= 0 && current.t < isect->t) {
                        *isect = current;
                    }
                }
            }
        } else {
            bool hitc0 = flatTree[ni + 1].bbox.intersect(ray, bbhits, bbhits + 1);
            bool hitc1 = flatTree[ni + node.rightOffset].bbox.intersect(ray, bbhits + 2, bbhits + 3);

            if (hitc0 && hitc1) {
                closer = ni + 1;
                other = ni + node.rightOffset;

                if (bbhits[2] < bbhits[0]) {
                    std::swap(bbhits[0], bbhits[2]);
                    std::swap(bbhits[1], bbhits[3]);
                    std::swap(closer, other);
                }

                todo[++stackptr] = BVHTraversal(other, bbhits[2]);
                todo[++stackptr] = BVHTraversal(closer, bbhits[0]);
            }

            else if (hitc0) {
                todo[++stackptr] = BVHTraversal(ni + 1, bbhits[0]);
            }

            else if (hitc1) {
                todo[++stackptr] = BVHTraversal(ni + node.rightOffset, bbhits[2]);
            }
        }
    }

    if (isect->shape != NULL) {
        isect->p = ray.origin + ray.dir * isect->t;
    }

    return isect->shape != NULL;
}

void BVH::build() {
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

    while (stackptr > 0) {
        BVHBuildEntry &bnode(todo[--stackptr]);
        uint32_t start = bnode.start;
        uint32_t end = bnode.end;
        uint32_t nPrims = end - start;

        nNodes++;
        node.start = start;
        node.nPrims = nPrims;
        node.rightOffset = Untouched;

        AABB bb((buildPrims)[start]->world_bound());
        AABB bc((buildPrims)[start]->centroid());
        for (uint32_t p = start + 1; p < end; ++p) {
            bb.extend((buildPrims)[p]->world_bound());
            bc.extend((buildPrims)[p]->centroid());
        }
        node.bbox = bb;

        if (nPrims <= leafSize) {
            node.rightOffset = 0;
            nLeafs++;
        }

        buildnodes.push_back(node);

        if (bnode.parent != 0xfffffffc) {
            buildnodes[bnode.parent].rightOffset--;

            // When this is the second touch, this is the right child.
            // The right child sets up the offset for the flat tree.
            if (buildnodes[bnode.parent].rightOffset == TouchedTwice) {
                buildnodes[bnode.parent].rightOffset = nNodes - 1 - bnode.parent;
            }
        }

        if (node.rightOffset == 0)
            continue;

        uint32_t split_dim = bc.largest_axis();

        float split_coord = .5f * (bc.pMin[split_dim] + bc.pMax[split_dim]);

        uint32_t mid = start;
        for (uint32_t i = start; i < end; ++i) {
            if ((buildPrims)[i]->centroid()[split_dim] < split_coord) {
                std::swap((buildPrims)[i], (buildPrims)[mid]);
                ++mid;
            }
        }

        if (mid == start || mid == end) {
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
    for (uint32_t n = 0; n < nNodes; ++n) {
        flatTree[n] = buildnodes[n];
    }
}

AABB BVH::world_bound() const {
    AABB bb;
    for (auto prim : buildPrims) {
        bb.extend(prim->world_bound());
    }
    return bb;
}

Vector3f BVH::centroid() const {
    // TO-DO
    return Vector3f();
}

bool BVH::intersect(const Ray &ray, SurfaceInteraction *isect) const {
    return intersect_main(ray, isect, false);
}

bool BVH::intersect_p(const Ray &ray) const {
    SurfaceInteraction isect;
    return intersect_main(ray, &isect, true);
}

} // namespace filianore
