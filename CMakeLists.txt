cmake_minimum_required(VERSION 3.1)
project(binary_tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp tree-vizualization/TreeGraph.h tree-vizualization/TreeGraphBuilder.h)
add_executable(binary_tree ${SOURCE_FILES} Tree.h)