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
include_directories(${CMAKE_BINARY_DIR})

set(MODULES
    ${MAGIC}/Modules/Magic.json
    ${PROJECT_SOURCE_DIR}/Test.json
    ${PROJECT_SOURCE_DIR}/TestLog.json
    ${PROJECT_SOURCE_DIR}/TestConfig.json
)

add_custom_target(BaseGen ALL)
add_custom_command(
    TARGET BaseGen
    COMMAND ${MAGIC}/Bin/Gen ${MODULES} Base
)

#Test
add_executable(ExampleBase Main.cpp)

