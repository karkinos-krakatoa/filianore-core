#ifndef _SHADING_FRAME_H
#define _SHADING_FRAME_H

#include "elemental.h"
#include "../maths/vec3_math.h"
#include "../maths/scalar.h"

namespace filianore
{

    class ShadingFrame
    {
    public:
        ShadingFrame() {}

        ShadingFrame(const StaticArray<float, 3> &_n)
            : n(_n)
        {
            CoordinateSystem<float>(n, &s, &t);
        }

        ShadingFrame(const StaticArray<float, 3> _n, const StaticArray<float, 3> _s, const StaticArray<float, 3> _t)
            : n(_n), s(_s), t(_t)
        {
        }

        StaticArray<float, 3> ToLocal(const StaticArray<float, 3> &v) const
        {
            return (StaticArray<float, 3>(Dot(v, s), Dot(v, t), Dot(v, n)).Normalize());
        }

        StaticArray<float, 3> ToWorld(const StaticArray<float, 3> &v) const
        {
            return ((s * v.x()) + (t * v.y()) + (n * v.z())).Normalize();
        }

        FILIANORE_INLINE static float CosTheta(const StaticArray<float, 3> &v) { return v.z(); }
        FILIANORE_INLINE static float Cos2Theta(const StaticArray<float, 3> &v) { return v.z() * v.z(); }
        FILIANORE_INLINE static float AbsCosTheta(const StaticArray<float, 3> &v) { return std::abs(v.z()); }
        FILIANORE_INLINE static float SinTheta(const StaticArray<float, 3> &v) { return std::sqrt(ShadingFrame::Sin2Theta(v)); }
        FILIANORE_INLINE static float Sin2Theta(const StaticArray<float, 3> &v) { return std::max(0.f, 1.f - ShadingFrame::Cos2Theta(v)); }
        FILIANORE_INLINE static float TanTheta(const StaticArray<float, 3> &v) { return SinTheta(v) / CosTheta(v); }
        FILIANORE_INLINE static float Tan2Theta(const StaticArray<float, 3> &v) { return Sin2Theta(v) / Cos2Theta(v); }

        FILIANORE_INLINE static float CosPhi(const StaticArray<float, 3> &w)
        {
            float sinTheta = SinTheta(w);
            return (sinTheta == 0.f) ? 1.f : Clamp<float>(w.x() / sinTheta, -1.f, 1.f);
        }

        FILIANORE_INLINE static float SinPhi(const StaticArray<float, 3> &w)
        {
            float sinTheta = SinTheta(w);
            return (sinTheta == 0.f) ? 0.f : Clamp<float>(w.y() / sinTheta, -1.f, 1.f);
        }

        FILIANORE_INLINE static float Cos2Phi(const StaticArray<float, 3> &w)
        {
            return CosPhi(w) * CosPhi(w);
        }

        FILIANORE_INLINE static float Sin2Phi(const StaticArray<float, 3> &w)
        {
            return SinPhi(w) * SinPhi(w);
        }

        FILIANORE_INLINE static StaticArray<float, 3> SphericalDirection(float sinTheta, float cosTheta, float phi)
        {
            return StaticArray<float, 3>(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
        }

        FILIANORE_INLINE static bool SameHemisphere(const StaticArray<float, 3> &w, const StaticArray<float, 3> &wp)
        {
            return (w.z() * wp.z()) > 0;
        }

        StaticArray<float, 3> n, s, t;
    };

} // namespace filianore

#endif