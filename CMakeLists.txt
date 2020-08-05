#-----------------------------------------------------------------------------------
# CMake Config
#-----------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)


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
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -flto")

set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")


#-----------------------------------------------------------------------------------
# Main Build
#-----------------------------------------------------------------------------------

add_library(filianore STATIC
    src/samplers/global.cpp
    src/samplers/pixel.cpp
    src/samplers/stratified.cpp
    src/shapes/triangle.cpp)

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
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/matrix.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/scalar.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/static_array.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/transform.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/maths/vec3_math.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/maths)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/aabb.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/camera.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/elemental.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/ray.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/sampler.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/sampling.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/shape.h
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/core/util.h
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/core)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/render/samplers/global.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/render/samplers/pixel.h 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/render/samplers/stratified.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/render/samplers)

install(FILES 
    ${FULL_CMAKE_CURRENT_SOURCE_DIR}/render/shapes/triangle.h 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/filianore/render/shapes)

install(TARGETS filianore
    EXPORT filianore_export
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)

install(EXPORT filianore_export FILE filianore-config.cmake 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/filianore)