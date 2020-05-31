#include <filianore/core/sampler.h>


namespace filianore
{

    template <typename T> void Sampler<T>::Startpixel(const StaticArray<T, 2>& p)
    {
        currentPixel = p;
        currentPixelSampleIndex  = 0;
        array1DOffset = array2DOffset = 0;
    }

    template <typename T> bool Sampler<T>::StartNextSample()
    {
        array1DOffset = array2DOffset = 0;
        return ++currentPixelSampleIndex < samplesPerPixel;
    }

    template <typename T> bool Sampler<T>::SetSampleNumber(int64_t sampleNum)
    {
        array1DOffset = array2DOffset = 0;
        currentPixelSampleIndex = sampleNum;
        return currentPixelSampleIndex < samplesPerPixel;
    }

    template <typename T> void Sampler<T>::Request1DArray(int n)
    {
        samples1DArraySizes.emplace_back(n);
        sampleArray1D.emplace_back(std::vector<T>(n * samplesPerPixel));
    }

    template <typename T> void Sampler<T>::Request2DArray(int n)
    {
        samples2DArraySizes.emplace_back(n);
        sampleArray2D.emplace_back(std::vector<StaticArray<T, 2>>(n * samplesPerPixel));
    }

    template <typename T> const T* Sampler<T>::Get1DArray(int n)
    {
        if (array1DOffset == sampleArray1D.size())
            return nullptr;
        return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
    }

    template <typename T> const StaticArray<T, 2>* Sampler<T>::Get2DArray(int n)
    {
        if (array2DOffset == sampleArray2D.size())
            return nullptr;
        return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
    }


    template void Sampler<float>::Startpixel(const StaticArray<float, 2>& p);
    template bool Sampler<float>::StartNextSample();
    template bool Sampler<float>::SetSampleNumber(int64_t sampleNum);
    template void Sampler<float>::Request1DArray(int n);
    template void Sampler<float>::Request2DArray(int n);
    template const float* Sampler<float>::Get1DArray(int n);
    template const StaticArray<float, 2>* Sampler<float>::Get2DArray(int n);

}