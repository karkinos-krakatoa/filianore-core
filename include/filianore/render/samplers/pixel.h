#ifndef _PIXEL_SAMPLER_H
#define _PIXEL_SAMPLER_H


#include "../core/sampler.h"
#include "../../ext/pcg32.h"


namespace filianore
{

    class PixelSampler : public Sampler
    {
    public:
        float Get1D();
        StaticArray<float, 2> Get2D();


        PixelSampler(int64_t _samplesPerPixel, int nSampledDimensions)
            : Sampler(_samplesPerPixel)
        {
            for (int i = 0; i < nSampledDimensions; ++i)
            {
                samples1D.emplace_back(std::vector<float>(_samplesPerPixel));
                samples2D.emplace_back(std::vector<StaticArray<float, 2>>(_samplesPerPixel));
            }
        }

        bool StartNextSample();

        bool SetSampleNumber(int64_t sampleNum);


    protected:
        std::vector<std::vector<float>> samples1D;
        std::vector<std::vector<StaticArray<float, 2>>> samples2D;
        int current1DDimension = 0, current2DDimension = 0;
        pcg32 _pcg32;

    };

}


#endif