#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "../maths/transform.h"
#include "shape.h"
#include <utility>

namespace filianore
{
    class Primitive
    {
    public:
        Primitive() {}

        virtual AABB WorldBound() const = 0;

        virtual bool Intersect(const Ray &ray, SurfaceInteraction *isect) const = 0;

        virtual bool IntersectP(const Ray &ray) const = 0;

        virtual StaticArray<float, 3> Centroid() const = 0;

        virtual void ComputeScatteringFunctions(SurfaceInteraction *isect) const = 0;

        virtual Material *GetMaterial() const = 0;

        virtual AreaIlluminant *GetAreaIlluminant() const = 0;
    };

    class GeometricPrimitive : public Primitive
    {
    public:
        GeometricPrimitive() {}

        GeometricPrimitive(std::shared_ptr<Shape> &_shape, const std::shared_ptr<Material> &_material)
            : shape(_shape), material(_material), areaIllum(nullptr)
        {
        }

        GeometricPrimitive(std::shared_ptr<Shape> &_shape, const std::shared_ptr<Material> &_material, const std::shared_ptr<AreaIlluminant> &_areaIllum)
            : shape(_shape), material(_material), areaIllum(_areaIllum)
        {
        }

        AABB WorldBound() const;

        bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;

        bool IntersectP(const Ray &ray) const;

        StaticArray<float, 3> Centroid() const;

        void ComputeScatteringFunctions(SurfaceInteraction *isect) const;

        Material *GetMaterial() const;

        virtual AreaIlluminant *GetAreaIlluminant() const;

    private:
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
        std::shared_ptr<AreaIlluminant> areaIllum;
    };

} // namespace filianore

#endif