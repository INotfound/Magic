cmake_minimum_required(VERSION 3.0)

project(Tests)
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
    ${PROJECT_SOURCE_DIR}/../ThirdParty/GoogleTest/googletest/include
)
include(${MAGIC}/Magic.cmake)


add_subdirectory(${PROJECT_SOURCE_DIR}/../ThirdParty/GoogleTest ${PROJECT_BINARY_DIR}/GoogleTest)

### TEST ###
set(TestSrc
    main.cpp
    UnitTrace.cpp
    UnitString.cpp
    UnitCrypto.cpp
    UnitStream.cpp
    UnitCompress.cpp
)
add_executable(Test ${TestSrc})
target_link_libraries(Test gtest Magic ${MAGIC_DEPEND_LIBRARY})
### TEST ###

add_executable(TestHttpClient HttpClient.cpp)
target_link_libraries(TestHttpClient Magic ${MAGIC_DEPEND_LIBRARY})

add_executable(TestWebServer WebServer.cpp)
target_link_libraries(TestWebServer Magic ${MAGIC_DEPEND_LIBRARY})