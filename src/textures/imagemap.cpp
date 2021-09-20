#include <iostream>

#include "filianore/color/principalspectrum.h"
#include "filianore/core/interaction.h"
#include "filianore/textures/imagemap.h"

#include "filianore/color/spectrumoperations.h"
#include "filianore/color/spectruminits.h"

namespace filianore
{

    ImageMapTexture::~ImageMapTexture()
    {
        delete data;
    }

    ImageMapTexture::ImageMapTexture()
        : data(nullptr),
          width(0), height(0), bytes_per_scanline(0)
    {
    }

    ImageMapTexture::ImageMapTexture(const char *filename)
    {
        auto components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    PrincipalSpectrum ImageMapTexture::Evaluate(const SurfaceInteraction &isect) const
    {
        if (data == nullptr)
            return PrincipalSpectrum(0.f);

        StaticArray<float, 2> uv = isect.uv;
        float u = Clamp(uv.params[0], 0.f, 1.f);
        float v = 1.f - Clamp(uv.params[1], 0.f, 1.f);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * width);

        if (i >= width)
            i = width - 1;
        if (j >= height)
            j = height - 1;

        const auto colorScale = 1.f / 255.f;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        StaticArray<float, 3> colorAtUV = StaticArray<float, 3>(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);

        return FromReflectanceRGB(colorAtUV);
    }
}