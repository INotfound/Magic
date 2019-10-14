project(Server)

cmake_minimum_required(VERSION 2.6)

add_subdirectory(Magic)

add_executable(Server main.cc)

target_link_libraries(Server Magic)
