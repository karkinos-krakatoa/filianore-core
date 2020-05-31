#pragma once


#include "pixel.h"


namespace filianore
{

    template <typename T> class StratifiedSampler : public PixelSampler<T>
    {
    public:
        StratifiedSampler(int _xPixelSamples, int _yPixelSamples, bool _jitterSamples,
                    int nSampledDimensions)
            : PixelSampler<T>(_xPixelSamples * _yPixelSamples, nSampledDimensions),
            xPixelSamples(_xPixelSamples), yPixelSamples(_yPixelSamples), 
            jitterSamples(_jitterSamples)
            { }

        void StartPixel(const StaticArray<T, 2>& p);


    private:
        void StratifiedSample1D(T* samp, int nSamples, pcg32& pcg32, bool jitter);
        void StratifiedSample2D(StaticArray<T, 2>* samp, int nx, int ny, pcg32& pcg32, bool jitter);

        const int xPixelSamples, yPixelSamples;
        const bool jitterSamples;
    };

}