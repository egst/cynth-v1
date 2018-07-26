cmake_minimum_required(VERSION 3.5)
project(Cynth CXX)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB_RECURSE SOURCES test.cpp src/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(Cynth ${SOURCES})