cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME Base)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Please Add The Directory Path Of The Magic Library.
set(MAGIC ${PROJECT_SOURCE_DIR}/../../../Magic)

if(NOT DEFINED MAGIC)
	message(FATAL_ERROR "Please Add The Directory Path Of The Magic Library!!!")
endif()

include_directories(
	${MAGIC}/Include
	${PROJECT_BINARY_DIR}/Include
	${MAGIC}/ThirdParty/Asio/Include
	${PROJECT_SOURCE_DIR}/../../../Magic/Examples
)
include(${MAGIC}/Magic.cmake)
link_directories(${MAGIC}/Lib)

set(MODULES
	${MAGIC}/Modules/Magic.json
	${PROJECT_SOURCE_DIR}/Test.json
	${PROJECT_SOURCE_DIR}/TestLog.json
	${PROJECT_SOURCE_DIR}/TestConfig.json
)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Include)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_custom_command(
	OUTPUT ${PROJECT_NAME}.h
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Include
	COMMAND ${MAGIC}/Bin/Gen ${MODULES} ${PROJECT_NAME}
)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.h ${SOURCES})
target_link_libraries(${PROJECT_NAME} Magic ${MAGIC_DEPEND_LIBRARY})