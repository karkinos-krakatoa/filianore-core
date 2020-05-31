#include <filianore/sampler/stratified.h>
#include <filianore/core/sampling.h>


namespace filianore
{

    template <typename T> void StratifiedSampler<T>::StratifiedSample1D(T* samp, int nSamples, pcg32& pcg32, bool jitter)
    {
        T invNSamples = (T)1 / nSamples;
        for(int i = 0; i < nSamples; ++i)
        {
            T delta = jitter ? pcg32.nextFloat() : (T)0.5;
            samp[i] = std::min((i + delta) * invNSamples, (T)OneMinusEpsilon);
        }
    }


    template <typename T> void StratifiedSampler<T>::StratifiedSample2D(StaticArray<T, 2>* samp, int nx, int ny, pcg32& pcg32, bool jitter)
    {
        T dx = (T)1 / nx;
        T dy = (T)1 / ny;
        for(int y = 0; y < ny; ++y)
        {
            for(int x = 0; x < nx; ++x)
            {
                T jx = jitter ? pcg32.nextFloat() : (T)0.5;
                T jy = jitter ? pcg32.nextFloat() : (T)0.5;
                samp->params[0] = std::min((x + jx) * dx, (T)OneMinusEpsilon);
                samp->params[1] = std::min((y + jy) * dy, (T)OneMinusEpsilon);
                ++samp;
            }
        }
    }


    template <typename T> void StratifiedSampler<T>::StartPixel(const StaticArray<T, 2>& p)
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


        PixelSampler<T>::StartPixel(p);
    }




}