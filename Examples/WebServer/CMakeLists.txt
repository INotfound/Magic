cmake_minimum_required(VERSION 3.0)

project(ExampleWebServer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAGIC ${PROJECT_SOURCE_DIR}/../../../Magic)
include_directories(
    ${MAGIC}
	${MAGIC}/Include
    ${PROJECT_SOURCE_DIR}
    ${MAGIC}/ThirdParty/Asio/Include
    ${MAGIC}/ThirdParty/RapidJSON/Include
)
link_directories(
    ${MAGIC}/Lib
)
link_libraries(Magic)
include(${MAGIC}/Magic.cmake)
include_directories(${CMAKE_BINARY_DIR})

set(MODULES
    ${MAGIC}/Modules/Magic.magic
    ${MAGIC}/Modules/WebServer.magic
    ${PROJECT_SOURCE_DIR}/TestServlet.magic
)

add_custom_target(Gen ALL)
add_custom_command(
    TARGET Gen
    COMMAND ${MAGIC}/Bin/Gen ${MODULES} WebServer
)

#ExampleWebServer
    add_executable(ExampleWebServer main.cpp TestServlet.cpp)
