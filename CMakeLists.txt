#-----------------------------------------------------------------------------------
# CMake Config
#-----------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14.0)


#-----------------------------------------------------------------------------------
# Project Config
#-----------------------------------------------------------------------------------

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message (FATAL_ERROR "In-source builds are not permitted; run CMake inside an empty build directory.")
endif ()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(filianore)

set (CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake/modules
)


#-----------------------------------------------------------------------------------
# Lang and Compile
#-----------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")

set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

INCLUDE (CheckCXXSourceCompiles)
INCLUDE (CheckCXXSourceRuns)


#-----------------------------------------------------------------------------------
# Aligned Memory Allocations
#-----------------------------------------------------------------------------------

CHECK_CXX_SOURCE_COMPILES ( "
#include <malloc.h>
int main() { void * ptr = _aligned_malloc(1024, 32); }
" HAVE__ALIGNED_MALLOC )

CHECK_CXX_SOURCE_COMPILES ( "
#include <stdlib.h>
int main() {
  void *ptr;
  posix_memalign(&ptr, 32, 1024);
} " HAVE_POSIX_MEMALIGN )

CHECK_CXX_SOURCE_COMPILES ( "
#include <malloc.h>
int main() {
    void *ptr = memalign(32, 1024);
} " HAVE_MEMALIGN )

IF ( HAVE__ALIGNED_MALLOC )
  ADD_DEFINITIONS ( -D FILIANORE_HAVE__ALIGNED_MALLOC )
ELSEIF ( HAVE_POSIX_MEMALIGN )
  ADD_DEFINITIONS ( -D FILIANORE_HAVE_POSIX_MEMALIGN )
ELSEIF ( HAVE_MEMALIGN )
  ADD_DEFINITIONS ( -D FILIANORE_HAVE_MEMALIGN )
ELSE ()
  MESSAGE ( SEND_ERROR "Unable to find a way to allocate aligned memory" )
ENDIF ()


#-----------------------------------------------------------------------------------
# Main Build
#-----------------------------------------------------------------------------------

add_library(filianore STATIC
    src/accel/bvh.cpp

    src/bvhx/nodeintersectors.cpp
    src/bvhx/primitiveintersectors.cpp
    src/bvhx/raytraverser.cpp
    src/bvhx/utils.cpp

    src/cameras/physical.cpp
  
    src/color/metallurgy.cpp
    src/color/spectruminits.cpp
    src/color/spectrumoperations.cpp

    src/core/bsdf.cpp
    src/core/illuminant.cpp
    src/core/integrator.cpp
    src/core/interaction.cpp
    src/core/material.cpp
    src/core/memory.cpp
    src/core/microfacet.cpp
    src/core/primitive.cpp
    src/core/sampling.cpp
    src/core/scene.cpp
    src/core/shape.cpp
    src/core/texture.cpp

    src/creators/shapecreator.cpp

    src/illuminants/point.cpp
    src/illuminants/spot.cpp
    src/illuminants/directional.cpp
    src/illuminants/diffusearea.cpp

    src/integrators/pathintegrator.cpp

    src/materials/lambert.cpp
    src/materials/standardsurface.cpp

    src/samplers/whitenoise.cpp

    src/shading/bxdfs/diffusetransmission.cpp
    src/shading/bxdfs/clearcoat.cpp
    src/shading/bxdfs/lambert.cpp
    src/shading/bxdfs/orennayar.cpp
    src/shading/bxdfs/microfacetreflection.cpp

    src/shading/closures/fresnelblendeddiffspec.cpp
    src/shading/closures/fresnelspecular.cpp 
    src/shading/closures/microfacettransmission.cpp

    src/shading/fresnel/fresneldielectric.cpp
    src/shading/fresnel/fresnelconductor.cpp
    src/shading/fresnel/fresnelnull.cpp
    src/shading/fresnel/schlickdielectric.cpp
    src/shading/fresnel/schlickmetallic.cpp
    src/shading/fresnel/thinfilm.cpp

    src/shading/microfacets/beckmann.cpp
    src/shading/microfacets/estevez.cpp
    src/shading/microfacets/ggx.cpp
    
    src/shapes/triangle.cpp

    src/textures/constant.cpp
    src/textures/imagemap.cpp)

target_include_directories(filianore PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


#---------------------------------------------------------------------------
# Add external Pkgs
#---------------------------------------------------------------------------

if (NOT MSVC)
    find_package(OpenMP REQUIRED)
    if (OpenMP_CXX_FOUND)
        message(STATUS "OpenMP found")
        target_link_libraries(filianore INTERFACE OpenMP::OpenMP_CXX)
    endif ()
else ()
    message(STATUS "OpenMP is disabled because MSVC only supports OpenMP 2.0.")
endif ()


#-----------------------------------------------------------------------------------
# Install and Export
#-----------------------------------------------------------------------------------

set(FULL_CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/filianore)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/utils.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/bvhx.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/topdownbuilder.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/binnedsahbuildtask.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/binnedsahbuilder.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/primitiveintersectionresult.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/primitiveintersectors.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/nodeintersectors.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/bvhx/raytraverser.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/bvhx)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/accel/bvh.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/accel)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/cameras/physical.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/cameras)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color/basespectrum.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color/cie.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color/metallurgy.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color/principalspectrum.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color/spectruminits.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color/spectrumoperations.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/color)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/aabb.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/bsdf.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/bxdf.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/camera.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/elemental.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/fresnel.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/illuminant.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/integrator.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/interaction.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/material.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/memory.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/microfacet.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/primitive.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/ray.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/rect.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/renderparams.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/sampler.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/sampling.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/scene.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/shadingcore.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/shape.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/texture.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/util.h
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/core)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/creators/shapecreator.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/creators/spdloader.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/creators)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/illuminants/point.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/illuminants/spot.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/illuminants/directional.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/illuminants/diffusearea.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/illuminants)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/integrators/pathintegrator.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/integrators)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/materials/lambert.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/materials/standardsurface.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/materials)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/matrix.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/scalar.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/static_array.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/transform.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/vec3_math.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/maths)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/samplers/whitenoise.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/samplers)

install(FILES  
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/bxdfs/diffusetransmission.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/bxdfs/clearcoat.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/bxdfs/orennayar.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/bxdfs/lambert.h  
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/bxdfs/microfacetreflection.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/bxdfs)

install(FILES  
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/closures/fresnelblendeddiffspec.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/closures/fresnelspecular.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/closures/microfacettransmission.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/closures)

install(FILES  
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel/fresneldielectric.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel/fresnelconductor.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel/fresnelnull.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel/schlickdielectric.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel/schlickmetallic.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel/thinfilm.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/fresnel)

install(FILES  
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/microfacets/beckmann.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/microfacets/estevez.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/microfacets/ggx.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/microfacets)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shapes/triangle.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shapes)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/textures/constant.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/textures/imagemap.h
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/textures)

install(TARGETS filianore
    EXPORT filianore_export
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)

install(EXPORT filianore_export FILE filianore-config.cmake 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)