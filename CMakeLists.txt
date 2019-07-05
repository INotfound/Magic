cmake_minimum_required(VERSION 2.6)
project(Main)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

add_definitions(-std=c++11)
add_subdirectory(Src)
aux_source_directory(. Local)

add_executable(Main ${Local})
target_link_libraries(Main Models)

