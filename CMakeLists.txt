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

set(FILIANORE_INCLUDE_PATH "src/filianore")
add_library(${PROJECT_NAME} 
    ${FILIANORE_INCLUDE_PATH}/core/elemental.cpp
    ${FILIANORE_INCLUDE_PATH}/math/vec.cpp)


add_library(sub::filianore ALIAS ${PROJECT_NAME})



target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)