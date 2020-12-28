#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include "elemental.h"
#include "primitive.h"
#include "aabb.h"
#include "illuminant.h"

namespace filianore
{

    class Scene
    {
    public:
        Scene(std::shared_ptr<Primitive> _primitive, const std::vector<std::shared_ptr<Illuminant>> &_illuminants)
            : primitive(_primitive), illuminants(_illuminants)
        {
            sceneBound = _primitive->WorldBound();

            for (const auto &illuminant : _illuminants)
            {
                illuminant->PrepareIlluminant(*this);
            }
        }

        const AABB &WorldBound() const { return sceneBound; }
        bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
        bool IntersectP(const Ray &ray) const;

        std::vector<std::shared_ptr<Illuminant>> illuminants;

    private:
        std::shared_ptr<Primitive> primitive;
        AABB sceneBound;
    };

} // namespace filianore

#endif