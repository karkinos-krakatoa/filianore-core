set (ACCEL_SOURCES
    src/filianore/bvh/flbvh.h
    src/filianore/bvh/flbvh.cpp 
)

set (CAMERA_SOURCES
    src/filianore/cameras/camera.h
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
    src/filianore/core/elemental.h
    src/filianore/core/interaction.h
    src/filianore/core/interaction.cpp
    src/filianore/core/primitive.h
    src/filianore/core/primitive.cpp
    src/filianore/core/renderparams.h
    src/filianore/core/scene.h
    src/filianore/core/scene.cpp
    src/filianore/core/shadingcore.h
)

set (CREATORS_SOURCES
    src/filianore/creators/spdloader.h
    src/filianore/creators/shapecreator.h
    src/filianore/creators/shapecreator.cpp
)

set (ILLUMS_SOURCES
    src/filianore/illuminants/illuminant.h
    src/filianore/illuminants/illuminant.cpp
    src/filianore/illuminants/point.h
    src/filianore/illuminants/point.cpp
    src/filianore/illuminants/spot.h
    src/filianore/illuminants/spot.cpp
    src/filianore/illuminants/directional.h
    src/filianore/illuminants/directional.cpp
    src/filianore/illuminants/diffusearea.h
    src/filianore/illuminants/diffusearea.cpp
)

set (INTEGRATORS_SOURCES
    src/filianore/integrators/integrator.h 
    src/filianore/integrators/integrator.cpp  
    src/filianore/integrators/pathintegrator.h 
    src/filianore/integrators/pathintegrator.cpp  
)

set (MATHS_SOURCES
    src/filianore/maths/affine.h
    src/filianore/maths/geometry.h
    src/filianore/maths/mathutils.h
)

set (MEMORY_SOURCES
    src/filianore/memory/memory.h
    src/filianore/memory/memory.cpp
)

set (SAMPLING_SOURCES
    src/filianore/sampling/sampler.h
    src/filianore/sampling/sampling.h
    src/filianore/sampling/sampling.cpp
)

set (SAMPLER_SOURCES
    src/filianore/sampling/samplers/whitenoise.h
    src/filianore/sampling/samplers/whitenoise.cpp
)

set (SHADING_SOURCES
    src/filianore/shading/bsdf.h
    src/filianore/shading/bsdf.cpp
)

set (SHADING_BXDFS_SOURCES
    src/filianore/shading/bxdfs/bxdf.h
    src/filianore/shading/bxdfs/diffusetransmission.h
    src/filianore/shading/bxdfs/diffusetransmission.cpp
    src/filianore/shading/bxdfs/clearcoat.h
    src/filianore/shading/bxdfs/clearcoat.cpp
    src/filianore/shading/bxdfs/lambert.h
    src/filianore/shading/bxdfs/lambert.cpp
    src/filianore/shading/bxdfs/orennayar.h
    src/filianore/shading/bxdfs/orennayar.cpp
    src/filianore/shading/bxdfs/microfacetreflection.h
    src/filianore/shading/bxdfs/microfacetreflection.cpp)

set (SHADING_CLOSURES_SOURCES
    src/filianore/shading/closures/fresnelblendeddiffspec.h
    src/filianore/shading/closures/fresnelblendeddiffspec.cpp
    src/filianore/shading/closures/fresnelspecular.h 
    src/filianore/shading/closures/fresnelspecular.cpp 
    src/filianore/shading/closures/microfacettransmission.h
    src/filianore/shading/closures/microfacettransmission.cpp)

set (SHADING_FRESNEL_SOURCES
    src/filianore/shading/fresnel/fresnel.h
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
    src/filianore/shading/fresnel/thinfilm.cpp)

set (SHADING_MATERIAL_SOURCES
    src/filianore/shading/materials/material.h
    src/filianore/shading/materials/material.cpp
    src/filianore/shading/materials/lambert.h
    src/filianore/shading/materials/lambert.cpp
    src/filianore/shading/materials/standardsurface.h 
    src/filianore/shading/materials/standardsurface.cpp 
)

set (SHADING_MICROFACETS_SOURCES
    src/filianore/shading/microfacets/microfacet.h
    src/filianore/shading/microfacets/microfacet.cpp
    src/filianore/shading/microfacets/beckmann.h
    src/filianore/shading/microfacets/beckmann.cpp
    src/filianore/shading/microfacets/estevez.h
    src/filianore/shading/microfacets/estevez.cpp
    src/filianore/shading/microfacets/ggx.h
    src/filianore/shading/microfacets/ggx.cpp
)

set (SHADING_TEXTURES_SOURCES
    src/filianore/shading/textures/texture.h
    src/filianore/shading/textures/constant.h
    src/filianore/shading/textures/imagemap.h
    src/filianore/shading/textures/imagemap.cpp
)

set (SHAPES_SOURCES
    src/filianore/shapes/shape.h
    src/filianore/shapes/shape.cpp
    src/filianore/shapes/triangle.h
    src/filianore/shapes/triangle.cpp
)



set(ALL_FILIANORE_SOURCES 
    ${ACCEL_SOURCES}
    ${CAMERA_SOURCES}
    ${COLOR_SOURCES}
    ${CORE_SOURCES}
    ${CREATORS_SOURCES}
    ${ILLUMS_SOURCES}
    ${INTEGRATORS_SOURCES}
    ${MATHS_SOURCES}
    ${MEMORY_SOURCES}
    ${SAMPLING_SOURCES}   
    ${SAMPLER_SOURCES}
    ${SHADING_SOURCES}
    ${SHADING_BXDFS_SOURCES}
    ${SHADING_CLOSURES_SOURCES}
    ${SHADING_FRESNEL_SOURCES}
    ${SHADING_MATERIAL_SOURCES}
    ${SHADING_MICROFACETS_SOURCES}
    ${SHADING_TEXTURES_SOURCES}
    ${SHAPES_SOURCES} PARENT_SCOPE)