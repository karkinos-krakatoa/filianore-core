#ifndef _GLOBAL_SAMPLER_H
#define _GLOBAL_SAMPLER_H


#include "../core/sampler.h"


namespace filianore
{

    class GlobalSampler : public Sampler
    {
    public:
        float Get1D();
        StaticArray<float, 2> Get2D();


        GlobalSampler(int64_t _samplesPerPixel)
            : Sampler(_samplesPerPixel)
        {}

        virtual int64_t GetIndexForSample(int64_t sampleNum) const = 0;

        virtual float SampleDimension(int64_t index, int dimension) const = 0;

        void Startpixel(const StaticArray<float, 2>& p);

        bool StartNextSample();

        bool SetSampleNumber(int64_t sampleNum);


    private:
        int dimension;
        int64_t intervalSampleIndex;
        static const int arrayStartDim = 5; //pixel+lens+time
        int arrayEndDim;

    };

}


#endif