# Android Compile Native API 24 Support Is Required.
cmake_minimum_required(VERSION 3.0)

project(Magic)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Magic.cmake)

include_directories(
    Include
    ThirdParty/Asio/Include
    ThirdParty/Brotli/c/include
    ThirdParty/RapidJSON/Include
)

if(NOT Sanitize)
    set(Analysis OFF)
else()
    set(ENABLE_SANITIZER ${Sanitize})
endif()

set(BROTLI_DISABLE_TESTS ON)
# PARENT_SCOPE Exporting Variables
add_subdirectory(ThirdParty/Brotli)
file(GLOB_RECURSE HEADERS Include/Magic/*.h)
file(GLOB_RECURSE SOURCES Source/Magic/*.cpp)

set(BINARY Magic)
add_library(Magic ${HEADERS} ${SOURCES})
target_link_libraries(Magic ${MAGIC_DEPEND_LIBRARY} ${BROTLI_STATIC_LIBS})

set(BINARY
    Gen
    Create
    ${BINARY}
)

add_executable(Gen Tool/Generator.cpp)
add_executable(Create Tool/Create.cpp)

if(Trace STREQUAL ON)
    add_definitions(-DTRACE)
endif()

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