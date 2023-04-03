#ifndef _WHITE_NOISE_H
#define _WHITE_NOISE_H

#include "../sampler.h"
#include <cstdlib>
#include <iostream>
#include <random>

namespace filianore {

class Whitenoise : public Sampler {
public:
    Whitenoise();

    float get_1d() {
        return dist(m_mersenneTwister);
    }

    Vector2f get_2d() {
        return Vector2f(get_1d(), get_1d());
    }

    void prepare_next_sample() {}

private:
    std::random_device rd;
    std::mt19937 m_mersenneTwister{rd()};
    std::uniform_real_distribution<float> dist{0.f, 1.f};
};

} // namespace filianore

#endif