#pragma once


#include <memory>
#include <vector>
#include <filianore/math/static_array.h>


namespace filianore
{

    static const float OneMinusEpsilon = 0x1.fffffep-1;

    class Sampler
    {
    public:
        Sampler(int64_t _samplesPerPixel)
        : samplesPerPixel(_samplesPerPixel) { }

        virtual ~Sampler(){ }

        virtual void StartPixel(const StaticArray<float, 2>& p);

        virtual float Get1D() = 0;
        virtual StaticArray<float, 2> Get2D() = 0;

        void Request1DArray(int n);
        void Request2DArray(int n);

        virtual int RoundCount(int n) const { return n; }

        const float* Get1DArray(int n);
        const StaticArray<float, 2>* Get2DArray(int n);

        virtual bool StartNextSample();

        virtual std::unique_ptr<Sampler> Clone(int seed) = 0;

        virtual bool SetSampleNumber(int64_t sampleNum);


        const int64_t samplesPerPixel;


    private:
        size_t array1DOffset, array2DOffset;


    protected:
        StaticArray<float, 2> currentPixel;
        int64_t currentPixelSampleIndex;
        std::vector<int> samples1DArraySizes, samples2DArraySizes;
        std::vector<std::vector<float>> sampleArray1D;
        std::vector<std::vector<StaticArray<float, 2>>> sampleArray2D;

    };
    
}