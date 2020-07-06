#ifndef _SAMPLER_H
#define _SAMPLER_H


#include <memory>
#include <vector>
#include "maths/static_array.h"


namespace filianore
{

    static const float OneMinusEpsilon = 0x1.fffffep-1;

    class Sampler
    {
    public:
        Sampler(int64_t _samplesPerPixel)
        : samplesPerPixel(_samplesPerPixel) { }

        virtual ~Sampler(){ }

        virtual void StartPixel(const StaticArray<float, 2>& p)
        {
            currentPixel = p;
            currentPixelSampleIndex  = 0;
            array1DOffset = array2DOffset = 0;
        }


        virtual float Get1D() = 0;
        virtual StaticArray<float, 2> Get2D() = 0;


        void Request1DArray(int n)
        {
            samples1DArraySizes.emplace_back(n);
            sampleArray1D.emplace_back(std::vector<float>(n * samplesPerPixel));
        }
        void Request2DArray(int n)
        {
            samples2DArraySizes.emplace_back(n);
            sampleArray2D.emplace_back(std::vector<StaticArray<float, 2>>(n * samplesPerPixel));
        }


        virtual int RoundCount(int n) const { return n; }


        const float* Get1DArray(int n)
        {
            if (array1DOffset == sampleArray1D.size())
                return nullptr;
            return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
        }
        const StaticArray<float, 2>* Get2DArray(int n)
        {
            if (array2DOffset == sampleArray2D.size())
                return nullptr;
            return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
        }


        virtual bool StartNextSample()
        {
            array1DOffset = array2DOffset = 0;
            return ++currentPixelSampleIndex < samplesPerPixel;
        }


        virtual std::unique_ptr<Sampler> Clone(int seed) = 0;


        virtual bool SetSampleNumber(int64_t sampleNum)
        {
            array1DOffset = array2DOffset = 0;
            currentPixelSampleIndex = sampleNum;
            return currentPixelSampleIndex < samplesPerPixel;
        }


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


#endif