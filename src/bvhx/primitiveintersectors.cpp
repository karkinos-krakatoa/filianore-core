#include "filianore/bvhx/primitiveintersectors.h"
#include "filianore/core/primitive.h"
#include "filianore/bvhx/bvhx.h"

namespace filianore
{

    std::optional<PrimitiveIntersectionResult> PrimitiveIntersector::Intersect(size_t index, const Ray &ray) const
    {
        auto [prim, idx] = this->PrimitiveAt(index);

        SurfaceInteraction isect;
        bool hit = prim->Intersect(ray, &isect);
        if (hit)
        {
            return std::make_optional(PrimitiveIntersectionResult{idx, isect});
        }

        return std::nullopt;
    }

    std::pair<const std::shared_ptr<Primitive> &, size_t> PrimitiveIntersector::PrimitiveAt(size_t index) const
    {
        index = bvh.primitiveIndices[index];
        return std::pair<const std::shared_ptr<Primitive> &, size_t>{primitives[index], index};
    }

}