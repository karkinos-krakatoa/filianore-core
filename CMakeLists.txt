# --------------------------------------------------------------------------------------------------
# Top-Level CMake
# --------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14...3.25)

project(filianore
    DESCRIPTION "Foundational Library for Filianore"
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --------------------------------------------------------------------------------------------------
#  Optional features available to users
# --------------------------------------------------------------------------------------------------

option(FL_ENABLE_EMBREE         "Use Embree for ray tracing operations" ON)
option(FL_INSTALL_LIBRARY       "Install Filianore library" OFF)

# --------------------------------------------------------------------------------------------------
# In-tree builds are not permitted
# --------------------------------------------------------------------------------------------------

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message (FATAL_ERROR "In-source builds are not permitted; run CMake inside an empty build directory.")
endif ()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


# --------------------------------------------------------------------------------------------------
# Handle and Build External Dependencies 
# --------------------------------------------------------------------------------------------------

add_subdirectory(external)

if (FL_ENABLE_EMBREE)
  add_definitions(-DFL_ENABLE_EMBREE=1)
  message(STATUS "Filianore : using Embree for CPU ray tracing.")
else()
  message(STATUS "Filianore : using built-in implementation for CPU ray tracing.")
endif()


# --------------------------------------------------------------------------------------------------
# Aligned Memory Allocations
# --------------------------------------------------------------------------------------------------

include (CheckCXXSourceCompiles)
include (CheckCXXSourceRuns)

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


# --------------------------------------------------------------------------------------------------
# Project Source & Build
# --------------------------------------------------------------------------------------------------

add_subdirectory(src)

add_library(filianore STATIC
    ${ALL_FILIANORE_SOURCES}
    )


# --------------------------------------------------------------------------------------------------
# Lang and Compile
# --------------------------------------------------------------------------------------------------

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


# --------------------------------------------------------------------------------------------------
# Include Directories
# --------------------------------------------------------------------------------------------------

target_include_directories(filianore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (FL_ENABLE_EMBREE)
  target_include_directories(filianore PUBLIC ${EMBREE_INCLUDE_DIRS})
endif()


# --------------------------------------------------------------------------------------------------
# Add Library dependencies
# --------------------------------------------------------------------------------------------------

if (FL_ENABLE_EMBREE)
  target_link_libraries(filianore PRIVATE embree)
endif()


# --------------------------------------------------------------------------------------------------
# Library Installation
# --------------------------------------------------------------------------------------------------

if(FL_INSTALL_LIBRARY)
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        install (
            TARGETS filianore
            RUNTIME
            DESTINATION bin
        )

        install (
            TARGETS filianore
            ARCHIVE
            DESTINATION lib
        )
    else ()
        install (
            TARGETS filianore
            DESTINATION lib
        )
    endif()
endif()

