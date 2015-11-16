cmake_minimum_required(VERSION 3.1)
project(vizualization)

add_definitions(-std=c++11)

set(HEADER_FILES
        tree-vizualization/TreeGraph.h
        tree-vizualization/TreeGraphBuilder.h)


set(SOURCE_FILES )
#add_library(vizualization STATIC ${SOURCE_FILES} ${HEADER_FILES})

#target_link_libraries(vizualization containers)