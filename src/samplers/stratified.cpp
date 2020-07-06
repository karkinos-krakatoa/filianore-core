#include "filianore/render/samplers/stratified.h"
#include "filianore/render/sampling.h"


namespace filianore
{

    void StratifiedSampler::StratifiedSample1D(float* samp, int nSamples, pcg32& pcg32, bool jitter)
    {
        float invNSamples = 1.f / nSamples;
        for(int i = 0; i < nSamples; ++i)
        {
            float delta = jitter ? pcg32.nextFloat() : 0.5f;
            samp[i] = std::min((i + delta) * invNSamples, OneMinusEpsilon);
        }
    }


    void StratifiedSampler::StratifiedSample2D(StaticArray<float, 2>* samp, int nx, int ny, pcg32& pcg32, bool jitter)
    {
        float dx = 1.f / nx;
        float dy = 1.f / ny;
        for(int y = 0; y < ny; ++y)
        {
            for(int x = 0; x < nx; ++x)
            {
                float jx = jitter ? pcg32.nextFloat() : 0.5f;
                float jy = jitter ? pcg32.nextFloat() : 0.5f;
                samp->params[0] = std::min((x + jx) * dx, OneMinusEpsilon);
                samp->params[1] = std::min((y + jy) * dy, OneMinusEpsilon);
                ++samp;
            }
        }
    }


    void StratifiedSampler::StartPixel(const StaticArray<float, 2>& p)
    {
        for(size_t i = 0; i < samples1D.size(); ++i)
        {
            StratifiedSample1D(&samples1D[i][0], xPixelSamples * yPixelSamples, _pcg32,
                    jitterSamples);
            Shuffle(&samples1D[i][0], xPixelSamples * yPixelSamples, 1, _pcg32);
        }

        for(size_t i = 0; i < samples2D.size(); ++i)
        {
            StratifiedSample2D(&samples2D[i][0], xPixelSamples, yPixelSamples, _pcg32,
                    jitterSamples);
            Shuffle(&samples2D[i][0], xPixelSamples * yPixelSamples, 1, _pcg32);
        }


        PixelSampler::StartPixel(p);
    }




}