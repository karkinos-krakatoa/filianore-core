#ifndef _SAMPLER_H
#define _SAMPLER_H

#include "../maths/vec.h"
#include <memory>
#include <vector>

namespace filianore {

class Sampler {
public:
    virtual ~Sampler() {}

    virtual float get_1d() = 0;

    virtual Vector2f get_2d() = 0;

    virtual void prepare_next_sample() = 0;
};

} // namespace filianore

#endif