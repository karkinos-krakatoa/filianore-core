#--------------------------------------------------------------------------------------------------
# Top-Level CMake
#--------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14...3.25)

project(filianore)

set(CMAKE_CXX_STANDARD 17)
include (CheckCXXSourceCompiles)
include (CheckCXXSourceRuns)


#-----------------------------------------------------------------------------------
# Project Config
#-----------------------------------------------------------------------------------

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message (FATAL_ERROR "In-source builds are not permitted; run CMake inside an empty build directory.")
endif ()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


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
# Project Sources
#-----------------------------------------------------------------------------------

set (ACCEL_SOURCES
    src/accel/bvh.cpp 
)

set (CAMERA_SOURCES
    src/cameras/physical.cpp 
)

set (COLOR_SOURCES
    src/color/metallurgy.cpp
    src/color/spectruminits.cpp
    src/color/spectrumoperations.cpp
)

set (CORE_SOURCES
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
)

set (CREATORS_SOURCES
    src/creators/shapecreator.cpp
)

set (ILLUMS_SOURCES
    src/illuminants/point.cpp
    src/illuminants/spot.cpp
    src/illuminants/directional.cpp
    src/illuminants/diffusearea.cpp
)

set (INTEGRATORS_SOURCES
    src/integrators/pathintegrator.cpp  
)

set (MATERIAL_SOURCES
    src/materials/lambert.cpp
    src/materials/standardsurface.cpp 
)

set (SAMPLER_SOURCES
    src/samplers/whitenoise.cpp
)

set (SHADING_SOURCES
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
)

set (SHAPES_SOURCES
    src/shapes/triangle.cpp
)

set (TEXTURES_SOURCES
    src/textures/imagemap.cpp
)


#-----------------------------------------------------------------------------------
# Main Build
#-----------------------------------------------------------------------------------

add_library(filianore STATIC
    ${ACCEL_SOURCES}
    ${CAMERA_SOURCES}
    ${COLOR_SOURCES}
    ${CORE_SOURCES}
    ${CREATORS_SOURCES}
    ${ILLUMS_SOURCES}
    ${INTEGRATORS_SOURCES}
    ${MATERIAL_SOURCES}
    ${SAMPLER_SOURCES}
    ${SHADING_SOURCES}
    ${SHAPES_SOURCES}
    ${TEXTURES_SOURCES}
    )

target_include_directories(filianore PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


# -----------------------------------------------------------------------------------
# Handle External Dependencies with CPM
# -----------------------------------------------------------------------------------




#-----------------------------------------------------------------------------------
# Lang and Compile
#-----------------------------------------------------------------------------------

if(MSVC)
    set(MSVC_FLAGS_DEBUG  "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
    set(MSVC_FLAGS_RELEASE "/MD /O2 /Ot /Oi /Oy /D /NDEBUG")
    add_compile_options(filianore PUBLIC "$<$<CONFIG:DEBUG>:${MSVC_FLAGS_DEBUG}>")
    add_compile_options(filianore PUBLIC "$<$<CONFIG:RELEASE>:${MSVC_FLAGS_RELEASE}>")
else()
    set_target_properties(filianore PROPERTIES
    C_COMPILER /usr/bin/gcc
    CXX_COMPILER /usr/bin/g++)

    set(NON_MSVC_FLAGS_DEBUG  "-g")
    set(NON_MSVC_FLAGS_RELEASE "-O3 -march=native -flto")
    add_compile_options(filianore PUBLIC "$<$<CONFIG:DEBUG>:${NON_MSVC_FLAGS_DEBUG}>")
    add_compile_options(filianore PUBLIC "$<$<CONFIG:RELEASE>:${NON_MSVC_FLAGS_RELEASE}>")
endif()

set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")


#-----------------------------------------------------------------------------------
# Export Lib Setup
#-----------------------------------------------------------------------------------

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install (TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION bin
    )

    install (TARGETS ${PROJECT_NAME}
        ARCHIVE
        DESTINATION lib
    )
else ()
    install (TARGETS ${PROJECT_NAME}
        DESTINATION lib
    )
endif ()


#-----------------------------------------------------------------------------------
# Install and Export
#-----------------------------------------------------------------------------------

set(FULL_CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/filianore)

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/accel
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/accel
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/cameras
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/cameras
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/color
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/color
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/core
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/creators
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/creators
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/illuminants
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/illuminants
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/integrators
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/integrators
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/materials
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/materials
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/maths
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/samplers
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/samplers
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/bxdfs
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/bxdfs
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/closures
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/closures
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/fresnel
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/fresnel
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/microfacets
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/microfacets
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/shapes
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/shapes
        FILES_MATCHING PATTERN "*.h"
    )

install (
        DIRECTORY ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shading/textures
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shading/textures
        FILES_MATCHING PATTERN "*.h"
    )

install(TARGETS filianore
    EXPORT filianore_export
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)

install(EXPORT filianore_export FILE filianore-config.cmake 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)