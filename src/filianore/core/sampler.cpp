#include "sampler.h"


namespace filianore
{

    void Sampler::StartPixel(const StaticArray<float, 2>& p)
    {
        currentPixel = p;
        currentPixelSampleIndex  = 0;
        array1DOffset = array2DOffset = 0;
    }

    bool Sampler::StartNextSample()
    {
        array1DOffset = array2DOffset = 0;
        return ++currentPixelSampleIndex < samplesPerPixel;
    }

    bool Sampler::SetSampleNumber(int64_t sampleNum)
    {
        array1DOffset = array2DOffset = 0;
        currentPixelSampleIndex = sampleNum;
        return currentPixelSampleIndex < samplesPerPixel;
    }

    void Sampler::Request1DArray(int n)
    {
        samples1DArraySizes.emplace_back(n);
        sampleArray1D.emplace_back(std::vector<float>(n * samplesPerPixel));
    }

    void Sampler::Request2DArray(int n)
    {
        samples2DArraySizes.emplace_back(n);
        sampleArray2D.emplace_back(std::vector<StaticArray<float, 2>>(n * samplesPerPixel));
    }

    const float* Sampler::Get1DArray(int n)
    {
        if (array1DOffset == sampleArray1D.size())
            return nullptr;
        return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
    }

    const StaticArray<float, 2>* Sampler::Get2DArray(int n)
    {
        if (array2DOffset == sampleArray2D.size())
            return nullptr;
        return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
    }

}