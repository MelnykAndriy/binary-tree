cmake_minimum_required(VERSION 3.1)
project(containers)

add_definitions(-std=c++11)

set(HEADER_FILES Tree.h)


set(SOURCE_FILES )

#add_library(containers STATIC ${SOURCE_FILES} ${HEADER_FILES})
# We don't need library  because we have only header files