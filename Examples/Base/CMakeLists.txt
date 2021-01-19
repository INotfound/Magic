cmake_minimum_required(VERSION 3.0)

project(ExampleBase)
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

GENEX(${MAGIC} Base MODULE
    ${MAGIC}/Modules/Magic.magic
    ${PROJECT_SOURCE_DIR}/Test.magic
    ${PROJECT_SOURCE_DIR}/TestLog.magic
    ${PROJECT_SOURCE_DIR}/TestConfig.magic
)

#Test
    add_executable(ExampleBase Main.cpp)

