#ifndef _RENDER_PARAMS_H
#define _RENDER_PARAMS_H

namespace filianore
{
    struct RenderParams
    {
        int samples;
        int diffuseRayDepth;
        int specularRayDepth;
    };
}

#endif