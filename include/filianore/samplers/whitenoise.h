#ifndef _WHITE_NOISE_H
#define _WHITE_NOISE_H

#include "../core/sampler.h"
#include <cstdlib>
#include <random>

namespace filianore
{

    class Whitenoise : public Sampler
    {
    public:
        Whitenoise() {}

        float Get1D()
        {
            (float)m_mersenneTwister() / (float)m_mersenneTwister.max();
        }

        StaticArray<float, 2> Get2D()
        {
            return StaticArray<float, 2>(Get1D(), Get1D());
        }

    private:
        std::mt19937 m_mersenneTwister;
    };

} // namespace filianore

#endif