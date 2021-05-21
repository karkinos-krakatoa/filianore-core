#include "filianore/core/scene.h"
#include "filianore/core/illuminant.h"
#include "filianore/core/primitive.h"
#include "filianore/bvhx/raytraverser.h"
#include "filianore/bvhx/primitiveintersectors.h"

namespace filianore
{
    Scene::Scene(
        const std::shared_ptr<RayTraverser> &_traverser,
        const std::shared_ptr<PrimitiveIntersector> &_primIntersector,
        const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
        : traverser(_traverser), primIntersector(_primIntersector), illuminants(_illuminants)
    {
        for (const auto &illuminant : illuminants)
        {
            illuminant->PrepareIlluminant(*this);
        }
    }

    bool Scene::Intersect(const Ray &ray, SurfaceInteraction *isect) const
    {
        auto result = traverser->Traverse(ray, *primIntersector);
        if (result)
        {
            *isect = result->isect;
            return true;
        }

        return false;
    }
}