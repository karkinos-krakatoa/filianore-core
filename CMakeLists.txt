cmake_minimum_required(VERSION 3.17)

project(filianore)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

<<<<<<< HEAD
set(FILIANORE_INCLUDE_PATH "src/filianore")

add_library(sub::filianore ALIAS ${PROJECT_NAME})

=======
add_library(${PROJECT_NAME} 
    src/core/elemental.cpp
    src/math/vec.cpp)

add_library(sub::filianore ALIAS ${PROJECT_NAME})


>>>>>>> 62145ffa3026e6fdf62e83a6c07e5eb9a30bc659
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)