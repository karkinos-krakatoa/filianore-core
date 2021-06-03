#ifndef _HENYEY_GREENSTEIN_PHASE_H
#define _HENYEY_GREENSTEIN_PHASE_H

#include "../core/phase.h"

namespace filianore
{

    class HenyeyGreenstein : public PhaseFunction
    {
    public:
        HenyeyGreenstein(float _g)
            : g(_g)
        {
        }

        float EvaluatePhase(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const;

    private:
        const float g;
    };

} // namespace filianore

#endif