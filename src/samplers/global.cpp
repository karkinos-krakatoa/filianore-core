#include "filianore/samplers/global.h"


namespace filianore
{

    void GlobalSampler::Startpixel(const StaticArray<float, 2>& p)
    {
        Sampler::StartPixel(p);
        dimension = 0;
        intervalSampleIndex  = GetIndexForSample(0);
        arrayEndDim = arrayStartDim + this->sampleArray1D.size() + 2 * this->sampleArray2D.size();

        for (size_t i = 0; i < this->samples1DArraySizes.size(); ++i)
        {
            int nSamples = this->samples1DArraySizes[i] * this->samplesPerPixel;
            for (int j = 0; j < nSamples; ++j)
            {
                int64_t index = GetIndexForSample(j);
                this->sampleArray1D[i][j] = SampleDimension(index, arrayStartDim + i);
            }
        }

        int dim = arrayStartDim + this->samples1DArraySizes.size();
        for (size_t i = 0; i < this->samples2DArraySizes.size(); ++i)
        {
            int nSamples = this->samples2DArraySizes[i] * this->samplesPerPixel;
            for (int j = 0; j < nSamples; ++j)
            {
                int64_t index = GetIndexForSample(j);
                this->sampleArray2D[i][j] = StaticArray<float, 2>(SampleDimension(index, dim), SampleDimension(index, dim + 1));
            }
        }
    }


    bool GlobalSampler::StartNextSample()
    {
        dimension = 0;
        intervalSampleIndex = GetIndexForSample(this->currentPixelSampleIndex + 1);
        return Sampler::StartNextSample();
    }


    bool GlobalSampler::SetSampleNumber(int64_t sampleNum)
    {
        dimension = 0;
        intervalSampleIndex = GetIndexForSample(sampleNum);
        return Sampler::SetSampleNumber(sampleNum);
    }


    float GlobalSampler::Get1D()
    {
        if (dimension >= arrayStartDim && dimension < arrayEndDim)
            dimension = arrayEndDim;
        return SampleDimension(intervalSampleIndex, dimension++);
    }

    StaticArray<float, 2> GlobalSampler::Get2D()
    {
        if (dimension + 1 >= arrayStartDim && dimension < arrayEndDim)
            dimension = arrayEndDim;
        StaticArray<float, 2> p(SampleDimension(intervalSampleIndex, dimension),
            SampleDimension(intervalSampleIndex, dimension + 1));
        dimension += 2;
        return p;
    }

}