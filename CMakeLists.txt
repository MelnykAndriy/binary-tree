cmake_minimum_required(VERSION 3.1)
project(binary_tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(containers)
include_directories(vizualization)

add_subdirectory(containers)
add_subdirectory(tests)
add_subdirectory(vizualization)
