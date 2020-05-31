#pragma once


#include <stdint.h>
#include <cmath>
#include <numeric>


namespace filianore
{

    #define PCG32_DEFAULT_STATE 0x853c49e6748fea9bULL
    #define PCG32_DEFAULT_STREAM 0xda3e39cb94b95bdbULL
    #define PCG32_MULT 0x5851f42d4c957f2dULL


    template <typename T> class RNG
    {
    public:
        RNG(): state(PCG32_DEFAULT_STATE), inc(PCG32_DEFAULT_STREAM) {}

        RNG(uint64_t _sequenceIndex)
        { 
            SetSequence(_sequenceIndex);
        }

        uint32_t UniformUInt32() 
        {
            uint64_t oldstate = state;
            state = oldstate * PCG32_MULT + inc;
            uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
            uint32_t rot = (uint32_t)(oldstate >> 59u);
            return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
        }

        uint32_t UniformUInt32(uint32_t b) 
        {
            uint32_t threshold = (~b + 1u) % b;
            while (true) 
            {
                uint32_t r = UniformUInt32();
                if (r >= threshold) 
                {
                    return r % b;
                }
            }
        }

        T UniformFloat() 
        {
            return std::min((T)0x1.fffffep-1, UniformUInt32() * (T)0x1p-32);
        }

    private:    
        void SetSequence(uint64_t sequenceIndex)
        {
            state = 0u;
            inc = (sequenceIndex << 1u) | 1u;
            UniformUInt32();
            state += PCG32_DEFAULT_STATE;
            UniformUInt32();
        }

        uint64_t state, inc;

    };

}