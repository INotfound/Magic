project(Server)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

add_definitions(
	#Not Security Check
	#-DNSC
	)

add_subdirectory(Magic/Log)

add_subdirectory(Magic/Thread)

add_subdirectory(Magic/Util)

add_subdirectory(Magic/Fiber)

aux_source_directory(. DIR_SRCS)

add_executable(Server ${DIR_SRCS})

target_link_libraries(Server Log Thread Util Fiber)
