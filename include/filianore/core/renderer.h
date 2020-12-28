#ifndef _RENDERER_H
#define _RENDERER_H

#include "../core/elemental.h"
#include "../maths/static_array.h"

namespace filianore
{
    typedef StaticArray<float, 3> Color;

    class TheRenderer
    {
    public:
        TheRenderer(int _maxDepth, std::shared_ptr<Camera> _camera, std::shared_ptr<Sampler> _sampler, float _rrThreshold = 1);
        void PrepareTheRenderer(const Scene &scene, Sampler &sampler);
        Color Li(const Ray &_ray, const Scene &scene, Sampler &sampler, int depth) const;

    private:
        const int maxDepth;
        const float rrThreshold;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Sampler> sampler;
    };

} // namespace filianore

#endif