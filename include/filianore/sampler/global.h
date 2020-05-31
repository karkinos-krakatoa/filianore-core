#pragma once


#include <filianore/core/sampler.h>


namespace filianore
{

    template <typename T> class GlobalSampler : public Sampler<T>
    {
    public:    
        GlobalSampler(int64_t _samplesPerPixel)
        : Sampler<T>(_samplesPerPixel)
        {}

        virtual int64_t GetIndexForSample(int64_t sampleNum) const = 0;

        virtual T SampleDimension(int64_t index, int dimension) const = 0;

        void Startpixel(const StaticArray<T, 2>& p);

        bool StartNextSample();

        bool SetSampleNumber(int64_t sampleNum);

        T Get1D();
        StaticArray<T, 2> Get2D();


    private:
        int dimension;
        int64_t intervalSampleIndex;
        static const int arrayStartDim = 5; //pixel+lens+time
        int arrayEndDim;

    };

}