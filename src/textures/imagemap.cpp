#include <iostream>

#include "filianore/color/principalspectrum.h"
#include "filianore/core/interaction.h"
#include "filianore/textures/imagemap.h"

#include "filianore/color/spectruminits.h"
#include "filianore/color/spectrumoperations.h"

#define STB_IMAGE_IMPLEMENTATION
#include "filianore/ext/stb_image.h"

namespace filianore {

ImageMapTexture::~ImageMapTexture() {
    delete data;
}

ImageMapTexture::ImageMapTexture()
    : data(nullptr),
      width(0), height(0), bytes_per_scanline(0) {
}

ImageMapTexture::ImageMapTexture(const char *filename) {
    auto components_per_pixel = bytes_per_pixel;
    data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

    if (!data) {
        std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

PrincipalSpectrum ImageMapTexture::evaluate(const SurfaceInteraction &isect) const {
    if (data == nullptr)
        return PrincipalSpectrum(0.f);

    Vector2f uv = isect.uv;

    int i = int(uv.x * width);
    int j = int((1.f - uv.y) * height - 0.001f);

    if (i < 0)
        i = 0;

    if (j < 0)
        j = 0;

    if (i > width - 1)
        i = width - 1;

    if (j > height - 1)
        i = height - 1;

    float r = int(data[3 * i + 3 * width * j]) / 255.f;
    float g = int(data[3 * i + 3 * width * j + 1]) / 255.f;
    float b = int(data[3 * i + 3 * width * j + 2]) / 255.f;

    return from_reflectance_rgb(Vector3f(r, g, b));
}

} // namespace filianore