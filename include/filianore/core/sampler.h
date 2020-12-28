#ifndef _SAMPLER_H
#define _SAMPLER_H

#include <memory>
#include <vector>
#include "../maths/static_array.h"

namespace filianore
{

    class Sampler
    {
    public:
        virtual ~Sampler() {}

        virtual float Get1D() = 0;

        virtual StaticArray<float, 2> Get2D() = 0;
    };

} // namespace filianore

#endif