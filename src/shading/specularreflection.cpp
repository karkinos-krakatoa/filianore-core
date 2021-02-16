#include "filianore/shading/specularreflection.h"
#include "filianore/core/shadingcore.h"

namespace filianore
{

    RGBSpectrum SpecularReflection::Evaluate(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return RGBSpectrum(0.f);
    }

    RGBSpectrum SpecularReflection::Sample(const StaticArray<float, 3> &wo, StaticArray<float, 3> *wi, const StaticArray<float, 2> &sample, float *pdf, BxDFType *sampledType) const
    {
        *wi = StaticArray<float, 3>(-wo.x(), -wo.y(), wo.z());
        *pdf = 1.f;
        *sampledType = this->bxDFType;

        return R;
    }

    float SpecularReflection::Pdf(const StaticArray<float, 3> &wo, const StaticArray<float, 3> &wi) const
    {
        return 0.f;
    }

} // namespace filianore