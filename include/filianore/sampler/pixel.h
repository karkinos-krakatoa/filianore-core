#pragma once


#include <filianore/core/sampler.h>
#include "../ext/pcg32/pcg32.h"


namespace filianore
{

    template <typename T> class PixelSampler : public Sampler<T>
    {
    public:
        PixelSampler(int64_t _samplesPerPixel, int nSampledDimensions)
        : Sampler<T>(_samplesPerPixel)
        {
            for(int i = 0; i < nSampledDimensions; ++i)
            {
                samples1D.emplace_back(std::vector<T>(_samplesPerPixel));
                samples2D.emplace_back(std::vector<StaticArray<T, 2>>(_samplesPerPixel));
            }
        }

        bool StartNextSample();

        bool SetSampleNumber(int64_t sampleNum);

        T Get1D();
        StaticArray<T, 2> Get2D();


    protected:
        std::vector<std::vector<T>> samples1D;
        std::vector<std::vector<StaticArray<T, 2>>> samples2D;
        int current1DDimension = 0, current2DDimension = 0;
        pcg32 _pcg32;
        
    };

}