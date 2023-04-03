#ifndef _BVH_H
#define _BVH_H

#include "../core/elemental.h"
#include "../core/primitive.h"
#include "../maths/geometry.h"
#include <vector>

namespace filianore {

struct BVHFlatNode {
    AABB bbox;
    uint32_t start, nPrims, rightOffset;
};

struct BVHTraversal {
    uint32_t i;
    float mint;
    BVHTraversal()
        : i(0), mint(0.f) {}

    BVHTraversal(int _i, float _mint)
        : i(_i), mint(_mint) {}
};

struct BVHBuildEntry {
    uint32_t parent;
    uint32_t start, end;
};

class BVH : public Primitive {
public:
    BVH() {}

    ~BVH() {
        delete[] flatTree;
    }

    BVH(const std::vector<std::shared_ptr<Primitive>> &primitives, uint8_t leafSize = 4)
        : nNodes(0), nLeafs(0), leafSize(leafSize), buildPrims(primitives), flatTree(nullptr) {
        build();
    }

    AABB world_bound() const;

    bool intersect(const Ray &ray, SurfaceInteraction *isect) const;

    bool intersect_p(const Ray &ray) const;

    Vector3f centroid() const;

    void compute_scattering_functions(SurfaceInteraction *isect) const {}

    Material *get_material() const { return nullptr; }

    AreaIlluminant *get_area_illuminant() const { return nullptr; }

private:
    uint32_t nNodes, nLeafs;
    uint8_t leafSize;
    std::vector<std::shared_ptr<Primitive>> buildPrims;
    BVHFlatNode *flatTree;

    void build();
    bool intersect_main(const Ray &ray, SurfaceInteraction *isect, bool occlusion = false) const;
};

} // namespace filianore

#endif
