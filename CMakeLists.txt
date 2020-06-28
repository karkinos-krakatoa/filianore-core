cmake_minimum_required(VERSION 3.17)

project(filianore)

set(CMAKE_CXX_STANDARD 17)


# set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
# set(OpenMP_C_LIB_NAMES "omp")
# set(OpenMP_omp_LIBRARY omp)

# set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
# set(OpenMP_CXX_LIB_NAMES "omp")
# set(OpenMP_omp_LIBRARY omp)

find_package(OpenMP QUIET)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(${PROJECT_NAME} INTERFACE ${OpenMP_CXX_LIBRARIES})
        target_include_directories(${PROJECT_NAME} INTERFACE ${OpenMP_CXX_INCLUDE_DIRS})
        target_link_options(${PROJECT_NAME} INTERFACE ${OpenMP_CXX_FLAGS})
        target_compile_options(${PROJECT_NAME} INTERFACE ${OpenMP_CXX_FLAGS})
    endif ()

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
    src/shapes/triangle.cpp)

add_library(sub::filianore ALIAS ${PROJECT_NAME})

#find_package (OpenMP REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)

