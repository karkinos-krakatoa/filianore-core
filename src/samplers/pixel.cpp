#include <filianore/sampler/pixel.h>


namespace filianore
{

    template <typename T> bool PixelSampler<T>::StartNextSample()
    {
        current1DDimension = current2DDimension = 0;
        return Sampler<T>::StartNextSample();
    }


    template <typename T> bool PixelSampler<T>::SetSampleNumber(int64_t sampleNum)
    {
        current1DDimension = current2DDimension = 0;
        return Sampler<T>::SetSampleNumber(sampleNum);
    }


    template <typename T> T PixelSampler<T>::Get1D()
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

    template <typename T> StaticArray<T, 2> PixelSampler<T>::Get2D()
    {
        if(current1DDimension < samples2D.size())
        {
            return samples2D[current1DDimension++][this->currentPixelSampleIndex];
        }
        else
        {
            return StaticArray<T, 2>(_pcg32.nextFloat(), _pcg32.nextFloat());
        }
    }



    template bool PixelSampler<float>::StartNextSample();
    template bool PixelSampler<float>::SetSampleNumber(int64_t sampleNum);
    template float PixelSampler<float>::Get1D();
    template StaticArray<float, 2> PixelSampler<float>::Get2D();

}