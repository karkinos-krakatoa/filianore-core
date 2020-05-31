#pragma once


#include <memory>
#include <vector>
#include <filianore/math/static_array.h>


namespace filianore
{

    static const float OneMinusEpsilon = 0x1.fffffep-1;

    template <typename T> class Sampler
    {
    public:
        Sampler(int64_t _samplesPerPixel)
        : samplesPerPixel(_samplesPerPixel) { }

        virtual void Startpixel(const StaticArray<T, 2>& p);

        virtual T Get1D() = 0;
        virtual StaticArray<T, 2> Get2D() = 0;

        void Request1DArray(int n);
        void Request2DArray(int n);

        virtual int RoundCount(int n) const { return n; }

        const T* Get1DArray(int n);
        const StaticArray<T, 2>* Get2DArray(int n);

        virtual bool StartNextSample();

        virtual std::unique_ptr<Sampler<T>> Clone(int seed) = 0;

        virtual bool SetSampleNumber(int64_t sampleNum);


        const int64_t samplesPerPixel;


    private:
        size_t array1DOffset, array2DOffset;


    protected:
        StaticArray<T, 2> currentPixel;
        int64_t currentPixelSampleIndex;
        std::vector<int> samples1DArraySizes, samples2DArraySizes;
        std::vector<std::vector<T>> sampleArray1D;
        std::vector<std::vector<StaticArray<T, 2>>> sampleArray2D;

    };
    
}