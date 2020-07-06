#include "filianore/render/samplers/pixel.h"


namespace filianore
{

    bool PixelSampler::StartNextSample()
    {
        current1DDimension = current2DDimension = 0;
        return Sampler::StartNextSample();
    }


    bool PixelSampler::SetSampleNumber(int64_t sampleNum)
    {
        current1DDimension = current2DDimension = 0;
        return Sampler::SetSampleNumber(sampleNum);
    }


    float PixelSampler::Get1D()
    {
        if(current1DDimension < samples1D.size())
        {
            return samples1D[current1DDimension++][this->currentPixelSampleIndex];
        }
        else
        {
            return _pcg32.nextFloat();
        }
    }

    StaticArray<float, 2> PixelSampler::Get2D()
    {
        if(current1DDimension < samples2D.size())
        {
            return samples2D[current1DDimension++][this->currentPixelSampleIndex];
        }
        else
        {
            return StaticArray<float, 2>(_pcg32.nextFloat(), _pcg32.nextFloat());
        }
    }


}