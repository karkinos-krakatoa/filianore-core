#-----------------------------------------------------------------------------------
# CMake Config
#-----------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17)


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
# Include Paths
#-----------------------------------------------------------------------------------

include_directories (
    src/middleware
    src/filianore
)


#-----------------------------------------------------------------------------------
# Main Build
#-----------------------------------------------------------------------------------

add_subdirectory (src/filianore)