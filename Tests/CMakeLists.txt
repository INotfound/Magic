cmake_minimum_required(VERSION 3.0)

project(Tests)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAGIC ${PROJECT_SOURCE_DIR}/..)

link_directories(
    ${MAGIC}/Lib
)
include_directories(
    ${MAGIC}
    ${MAGIC}/Include
    ${PROJECT_SOURCE_DIR}
    ${MAGIC}/ThirdParty/Asio/Include
    ${MAGIC}/ThirdParty/RapidJSON/Include
)
include(${MAGIC}/Magic.cmake)

### TEST ###
add_executable(Test main.cpp)
target_link_libraries(Test Magic ${MAGIC_DEPEND_LIBRARY})

message(${MAGIC_DEPEND_LIBRARY})

add_executable(HttpClient HttpClient.cpp)
target_link_libraries(HttpClient Magic ${MAGIC_DEPEND_LIBRARY})

add_executable(WebServer WebServer.cpp)
target_link_libraries(WebServer Magic ${MAGIC_DEPEND_LIBRARY})
### TEST ###