#include "filianore/media/henyeygreenstein.h"

namespace filianore {

float HenyeyGreenstein::EvaluatePhase(const Vector3f &wo, const Vector3f &wi) const {
    return PhaseHenyeyGreenstein(dot(wo, wi), g);
}
} // namespace filianore