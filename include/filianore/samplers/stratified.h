#ifndef _STRATIFIED_SAMPLER_H
#define _STRATIFIED_SAMPLER_H


#include "pixel.h"


namespace filianore
{

    class StratifiedSampler : public PixelSampler
    {
    public:
        StratifiedSampler(int _xPixelSamples, int _yPixelSamples, bool _jitterSamples,
            int nSampledDimensions)
            : PixelSampler(_xPixelSamples * _yPixelSamples, nSampledDimensions),
            xPixelSamples(_xPixelSamples), yPixelSamples(_yPixelSamples),
            jitterSamples(_jitterSamples)
        { }

        void StartPixel(const StaticArray<float, 2>& p);


    private:
        void StratifiedSample1D(float* samp, int nSamples, pcg32& pcg32, bool jitter);
        void StratifiedSample2D(StaticArray<float, 2>* samp, int nx, int ny, pcg32& pcg32, bool jitter);

        const int xPixelSamples, yPixelSamples;
        const bool jitterSamples;
    };

}


#endif