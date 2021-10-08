#ifndef _TEXTURE_IMAGE_MAP_H
#define _TEXTURE_IMAGE_MAP_H

#include "../core/elemental.h"
#include "../core/texture.h"
#include "../maths/static_array.h"

namespace filianore
{

    class ImageMapTexture : public Texture<PrincipalSpectrum>
    {
    public:
        const static int bytes_per_pixel = 3;

        ~ImageMapTexture();

        ImageMapTexture();
        ImageMapTexture(const char *filename);

        PrincipalSpectrum Evaluate(const SurfaceInteraction &isect) const;

    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;
    };

} // namespace filianore

#endif