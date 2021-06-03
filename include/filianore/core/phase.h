#ifndef _PHASE_FUNCTION_H
#define _PHASE_FUNCTION_H

#include "elemental.h"
#include "../maths/static_array.h"
#include "../maths/scalar.h"

namespace filianore
{

    inline float PhaseHenyeyGreenstein(float cosTheta, float g)
    {
        float denom = 1 + g * g + 2 * g * cosTheta;
        return (1 / (4 * Pi<float>)) * (1 - g * g) / (denom * std::sqrt(denom));
    }

    class PhaseFunction
    {
    public:
        virtual ~PhaseFunction() {}

        virtual float EvaluatePhase(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const = 0;
    };

} // namespace filianore

#endif