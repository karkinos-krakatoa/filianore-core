cmake_minimum_required(VERSION 3.17)

project(filianore)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} src/elemental.cpp)

target_include_directories(filianore PUBLIC ${PROJECT_SOURCE_DIR}/include)