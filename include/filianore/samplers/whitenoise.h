#ifndef _WHITE_NOISE_H
#define _WHITE_NOISE_H

#include "../core/sampler.h"
#include <cstdlib>
#include <iostream>
#include <random>

namespace filianore
{

    class Whitenoise : public Sampler
    {
    public:
        Whitenoise();

        float Get1D()
        {
            return dist(m_mersenneTwister);
        }

        StaticArray<float, 2> Get2D()
        {
            return StaticArray<float, 2>(Get1D(), Get1D());
        }

        void PrepareNextSample() {}

    private:
        std::random_device rd;
        std::mt19937 m_mersenneTwister{rd()};
        std::uniform_real_distribution<float> dist{0.f, 1.f};
    };

} // namespace filianore

#endif