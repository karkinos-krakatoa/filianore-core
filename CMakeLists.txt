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


#-----------------------------------------------------------------------------------
# Main Build
#-----------------------------------------------------------------------------------

add_library(filianore STATIC
    src/accel/bvh.cpp

    src/cameras/physical.cpp

    src/core/bsdf.cpp
    src/core/illuminant.cpp
    src/core/integrator.cpp
    src/core/interaction.cpp
    src/core/material.cpp
    src/core/memory.cpp
    src/core/primitive.cpp
    src/core/scene.cpp
    src/core/texture.cpp

    src/illuminants/point.cpp

    src/illuminants/path.cpp

    src/materials/lambert.cpp

    src/samplers/whitenoise.cpp

    src/shapes/triangle.cpp

    src/textures/constant.cpp)

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
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/accel/bvh.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/accel)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/cameras/physical.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/cameras)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/aabb.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/bsdf.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/camera.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/elemental.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/illuminant.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/integrator.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/interaction.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/material.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/memory.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/primitive.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/ray.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/rect.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/sampler.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/sampling.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/scene.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/shadingframe.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/shape.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/texture.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/util.h
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/core)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/illuminants/point.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/illuminants)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/integrators/path.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/integrators)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/materials/lambert.h 
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
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/shapes/triangle.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/shapes)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/textures/constant.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/textures)

install(TARGETS filianore
    EXPORT filianore_export
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)

install(EXPORT filianore_export FILE filianore-config.cmake 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)