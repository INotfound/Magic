# Android Compile Native API 24 Support Is Required.
cmake_minimum_required(VERSION 3.0)

project(Magic)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Magic.cmake)

include_directories(
    Include
    ThirdParty/Asio/Include
    ThirdParty/RapidJSON/Include
)

file(GLOB_RECURSE HEADERS Include/Magic/*.h)
file(GLOB_RECURSE SOURCES Source/Magic/*.cpp)

set(BINARY Magic)
add_library(Magic ${HEADERS} ${SOURCES})
target_link_libraries(Magic ${MAGIC_DEPEND_LIBRARY})
if(Sanitize STREQUAL Leak)
    set(Analysis OFF)
elseif(Sanitize STREQUAL Thread)
    set(Analysis OFF)
elseif(Sanitize STREQUAL Address)
    set(Analysis OFF)
elseif(Sanitize STREQUAL Undefined)
    set(Analysis OFF)
endif()

set(BINARY
    Gen
    Create
    ${BINARY}
)

add_executable(Gen Tool/Generator.cpp)
add_executable(Create Tool/Create.cpp)

if(Analysis STREQUAL ON)
    if(DEFINED MAGIC_CLANG_INCLUDE AND DEFINED MAGIC_CLANG_LIBRARY)
        set(BINARY ${BINARY} Analysis)
        link_directories(${MAGIC_CLANG_LIBRARY})
        include_directories(${MAGIC_CLANG_INCLUDE})
        add_executable(Analysis Tool/Analysis.cpp)
        target_link_libraries(Analysis clang)
    endif()
endif()

install(TARGETS ${BINARY}
    RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/Bin
    ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/Lib
)

if(Tests STREQUAL ON)
    add_subdirectory(Tests)
endif()

if(Examples STREQUAL ON)
    add_subdirectory(Examples/Base)
    add_subdirectory(Examples/WebServer)
endif()