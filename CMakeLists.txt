cmake_minimum_required(VERSION 3.17)

project(filianore)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -flto")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

add_library(${PROJECT_NAME} 
    src/core/elemental.cpp
    src/core/sampler.cpp
    src/samplers/pixel.cpp
    src/samplers/global.cpp
    src/shapes/triangle.cpp
    src/cameras/physical.cpp)

add_library(sub::filianore ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
