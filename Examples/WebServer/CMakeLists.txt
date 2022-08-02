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
    ${MAGIC}/Modules/Magic.json
    ${MAGIC}/Modules/WebServer.json
    ${PROJECT_SOURCE_DIR}/TestServlet.json
)

add_custom_target(WebServerGen ALL)
add_custom_command(
    TARGET WebServerGen
    COMMAND ${MAGIC}/Bin/Gen ${MODULES} WebServer
)

#ExampleWebServer
add_executable(ExampleWebServer main.cpp TestServlet.cpp)
