cmake_minimum_required(VERSION 3.0)

project(ExampleWebServer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MAGIC ../../../Magic)
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

GENEX(${MAGIC} WebServer MODULE
    ${MAGIC}/Magic.magic
    ${MAGIC}/WebServer.magic
    ${PROJECT_SOURCE_DIR}/TestServlet.magic
)

#ExampleWebServer
    add_executable(ExampleWebServer main.cpp TestServlet.cpp)