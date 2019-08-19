project(Server)

cmake_minimum_required(VERSION 2.6)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

add_subdirectory(Magic/Log)

add_subdirectory(Magic/Thread)

add_subdirectory(Magic/Util)

aux_source_directory(. DIR_SRCS)

add_executable(Server ${DIR_SRCS})

target_link_libraries(Server Log Thread Util)