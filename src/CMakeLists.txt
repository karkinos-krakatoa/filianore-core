set (ACCEL_SOURCES
    src/filianore/accel/bvh.h
    src/filianore/accel/bvh.cpp 
)

set (CAMERA_SOURCES
    src/filianore/cameras/physical.h 
    src/filianore/cameras/physical.cpp 
)

set (COLOR_SOURCES
    src/filianore/color/basespectrum.h
    src/filianore/color/cie.h
    src/filianore/color/metallurgy.h
    src/filianore/color/metallurgy.cpp
    src/filianore/color/principalspectrum.h
    src/filianore/color/spectruminits.h
    src/filianore/color/spectruminits.cpp
    src/filianore/color/spectrumoperations.h
    src/filianore/color/spectrumoperations.cpp
)

set (CORE_SOURCES
    src/filianore/core/aabb.h
    src/filianore/core/bsdf.h
    src/filianore/core/bsdf.cpp
    src/filianore/core/bxdf.h
    src/filianore/core/camera.h
    src/filianore/core/elemental.h
    src/filianore/core/fresnel.h
    src/filianore/core/illuminant.h
    src/filianore/core/illuminant.cpp
    src/filianore/core/integrator.h
    src/filianore/core/integrator.cpp
    src/filianore/core/interaction.h
    src/filianore/core/interaction.cpp
    src/filianore/core/material.h
    src/filianore/core/material.cpp
    src/filianore/core/memory.h
    src/filianore/core/memory.cpp
    src/filianore/core/microfacet.h
    src/filianore/core/microfacet.cpp
    src/filianore/core/primitive.h
    src/filianore/core/primitive.cpp
    src/filianore/core/ray.h
    src/filianore/core/rect.h
    src/filianore/core/renderparams.h
    src/filianore/core/sampler.h
    src/filianore/core/sampling.h
    src/filianore/core/sampling.cpp
    src/filianore/core/scene.h
    src/filianore/core/scene.cpp
    src/filianore/core/shadingcore.h
    src/filianore/core/shape.h
    src/filianore/core/shape.cpp
    src/filianore/core/texture.h
    src/filianore/core/util.h
)

set (CREATORS_SOURCES
    src/filianore/creators/spdloader.h
    src/filianore/creators/shapecreator.h
    src/filianore/creators/shapecreator.cpp
)

set (ILLUMS_SOURCES
    src/filianore/illuminants/point.h
    src/filianore/illuminants/point.cpp
    src/filianore/illuminants/spot.h
    src/filianore/illuminants/spot.cpp
    src/filianore/illuminants/directional.h
    src/filianore/illuminants/directional.cpp
    src/filianore/illuminants/diffusearea.cpp
    src/filianore/illuminants/diffusearea.cpp
)

set (INTEGRATORS_SOURCES
    src/filianore/integrators/pathintegrator.h 
    src/filianore/integrators/pathintegrator.cpp  
)

set (MATERIAL_SOURCES
    src/filianore/materials/lambert.h
    src/filianore/materials/lambert.cpp
    src/filianore/materials/standardsurface.h 
    src/filianore/materials/standardsurface.cpp 
)

set (MATHS_SOURCES
    src/filianore/maths/mathutils.h
    src/filianore/maths/matrix.h
    src/filianore/maths/transform.h
    src/filianore/maths/vec.h
)

set (SAMPLER_SOURCES
    src/filianore/samplers/whitenoise.h
    src/filianore/samplers/whitenoise.cpp
)

set (SHADING_SOURCES
    src/filianore/shading/bxdfs/diffusetransmission.h
    src/filianore/shading/bxdfs/diffusetransmission.cpp
    src/filianore/shading/bxdfs/clearcoat.h
    src/filianore/shading/bxdfs/clearcoat.cpp
    src/filianore/shading/bxdfs/lambert.h
    src/filianore/shading/bxdfs/lambert.cpp
    src/filianore/shading/bxdfs/orennayar.h
    src/filianore/shading/bxdfs/orennayar.cpp
    src/filianore/shading/bxdfs/microfacetreflection.h
    src/filianore/shading/bxdfs/microfacetreflection.cpp

    src/filianore/shading/closures/fresnelblendeddiffspec.h
    src/filianore/shading/closures/fresnelblendeddiffspec.cpp
    src/filianore/shading/closures/fresnelspecular.h 
    src/filianore/shading/closures/fresnelspecular.cpp 
    src/filianore/shading/closures/microfacettransmission.h
    src/filianore/shading/closures/microfacettransmission.cpp

    src/filianore/shading/fresnel/fresneldielectric.h
    src/filianore/shading/fresnel/fresneldielectric.cpp
    src/filianore/shading/fresnel/fresnelconductor.h
    src/filianore/shading/fresnel/fresnelconductor.cpp
    src/filianore/shading/fresnel/fresnelnull.h
    src/filianore/shading/fresnel/fresnelnull.cpp
    src/filianore/shading/fresnel/schlickdielectric.h
    src/filianore/shading/fresnel/schlickdielectric.cpp
    src/filianore/shading/fresnel/schlickmetallic.h
    src/filianore/shading/fresnel/schlickmetallic.cpp
    src/filianore/shading/fresnel/thinfilm.h
    src/filianore/shading/fresnel/thinfilm.cpp

    src/filianore/shading/microfacets/beckmann.h
    src/filianore/shading/microfacets/beckmann.cpp
    src/filianore/shading/microfacets/estevez.h
    src/filianore/shading/microfacets/estevez.cpp
    src/filianore/shading/microfacets/ggx.h
    src/filianore/shading/microfacets/ggx.cpp
)

set (SHAPES_SOURCES
    src/filianore/shapes/triangle.h
    src/filianore/shapes/triangle.cpp
)

set (TEXTURES_SOURCES
    src/filianore/textures/constant.h
    src/filianore/textures/imagemap.h
    src/filianore/textures/imagemap.cpp
)


set(ALL_FILIANORE_SOURCES 
    ${ACCEL_SOURCES}
    ${CAMERA_SOURCES}
    ${COLOR_SOURCES}
    ${CORE_SOURCES}
    ${CREATORS_SOURCES}
    ${ILLUMS_SOURCES}
    ${INTEGRATORS_SOURCES}
    ${MATERIAL_SOURCES}
    ${MATHS_SOURCES}
    ${SAMPLER_SOURCES}
    ${SHADING_SOURCES}
    ${SHAPES_SOURCES}
    ${TEXTURES_SOURCES} PARENT_SCOPE)