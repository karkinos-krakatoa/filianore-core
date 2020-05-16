cmake_minimum_required(VERSION 3.17)

project(filianore)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} src/elemental.cpp)
add_library(sub::filianore ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)