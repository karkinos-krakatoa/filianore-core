#ifndef _BVH_H
#define _BVH_H

#include <vector>
#include "../core/elemental.h"
#include "../core/primitive.h"
#include "../core/aabb.h"

namespace filianore
{

    struct BVHFlatNode
    {
        AABB bbox;
        uint32_t start, nPrims, rightOffset;
    };

    struct BVHTraversal
    {
        uint32_t i;
        float mint;
        BVHTraversal()
            : i(0), mint(0.f) {}

        BVHTraversal(int _i, float _mint)
            : i(_i), mint(_mint) {}
    };

    struct BVHBuildEntry
    {
        uint32_t parent;
        uint32_t start, end;
    };

    class BVH : public Primitive
    {
    public:
        BVH() {}

        ~BVH()
        {
            delete[] flatTree;
        }

        BVH(const std::vector<std::shared_ptr<Primitive>> &primitives, uint8_t leafSize = 4)
            : nNodes(0), nLeafs(0), leafSize(leafSize), buildPrims(primitives), flatTree(nullptr)
        {
            Build();
        }

        AABB WorldBound() const;

        bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;

        bool IntersectP(const Ray &ray) const;

        StaticArray<float, 3> Centroid() const;

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const {}

        Material *GetMaterial() const { return nullptr; }

        AreaIlluminant *GetAreaIlluminant() const { return nullptr; }

    private:
        uint32_t nNodes,
            nLeafs;
        uint8_t leafSize;
        std::vector<std::shared_ptr<Primitive>> buildPrims;
        BVHFlatNode *flatTree;

        void Build();
        bool IntersectMain(const Ray &ray, SurfaceInteraction *isect, bool occlusion = false) const;
    };

} // namespace filianore

#endif
