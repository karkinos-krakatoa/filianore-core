#include <filianore/sampler/global.h>


namespace filianore
{

    template <typename T> void GlobalSampler<T>::Startpixel(const StaticArray<T, 2>& p)
    {
        Sampler<T>::StartPixel(p);
        dimension = 0;
        intervalSampleIndex  = GetIndexForSample(0);
        arrayEndDim = arrayStartDim + this->sampleArray1D.size() + 2 * this->sampleArray2D.size();

        for(size_t i = 0; i < this->samples1DArraySizes.size(); ++i)
        {
            int nSamples = this->samples1DArraySizes[i] * this->samplesPerPixel;
            for(int j = 0; j < nSamples; ++j)
            {
                int64_t index = GetIndexForSample(j);
                this->sampleArray1D[i][j] = SampleDimension(index, arrayStartDim + i);
            }
        }

        int dim = arrayStartDim + this->samples1DArraySizes.size();
        for(size_t i = 0; i < this->samples2DArraySizes.size(); ++i)
        {
            int nSamples = this->samples2DArraySizes[i] * this->samplesPerPixel;
            for(int j = 0; j < nSamples; ++j)
            {
                int64_t index = GetIndexForSample(j);
                this->sampleArray2D[i][j] = StaticArray<T, 2>(SampleDimension(index, dim), SampleDimension(index, dim + 1));
            }
        }
    }


    template <typename T> bool GlobalSampler<T>::StartNextSample()
    {
        dimension = 0;
        intervalSampleIndex = GetIndexForSample(this->currentPixelSampleIndex + 1);
        return Sampler<T>::StartNextSample();
    }


    template <typename T> bool GlobalSampler<T>::SetSampleNumber(int64_t sampleNum)
    {
        dimension = 0;
        intervalSampleIndex = GetIndexForSample(sampleNum);
        return Sampler<T>::SetSampleNumber(sampleNum);
    }


    template <typename T> T GlobalSampler<T>::Get1D()
    {
        if (dimension >= arrayStartDim && dimension < arrayEndDim)
            dimension = arrayEndDim;
        return SampleDimension(intervalSampleIndex, dimension++);
    }

    template <typename T> StaticArray<T, 2> GlobalSampler<T>::Get2D()
    {
        if (dimension + 1 >= arrayStartDim && dimension < arrayEndDim)
            dimension = arrayEndDim;
        StaticArray<T, 2> p(SampleDimension(intervalSampleIndex, dimension),
              SampleDimension(intervalSampleIndex, dimension + 1));
        dimension += 2;
        return p;
    }



    template void GlobalSampler<float>::Startpixel(const StaticArray<float, 2>& p);
    template bool GlobalSampler<float>::StartNextSample();
    template bool GlobalSampler<float>::SetSampleNumber(int64_t sampleNum);
    template float GlobalSampler<float>::Get1D();
    template StaticArray<float, 2> GlobalSampler<float>::Get2D();

}