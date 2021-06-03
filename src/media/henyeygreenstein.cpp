#include "filianore/media/henyeygreenstein.h"

namespace filianore
{

    float HenyeyGreenstein::EvaluatePhase(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return PhaseHenyeyGreenstein(Dot(wo, wi), g);
    }
}