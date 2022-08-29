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

set(SOURCES
    Source/Magic/Magic.cpp
# Utilty
    Source/Magic/Utilty/Gzip.cpp
    Source/Magic/Utilty/Trace.cpp
    Source/Magic/Utilty/Timer.cpp
    Source/Magic/Utilty/Crypto.cpp
    Source/Magic/Utilty/Logger.cpp
    Source/Magic/Utilty/Config.cpp
    Source/Magic/Utilty/Thread.cpp
    Source/Magic/Utilty/TimingWheel.cpp
# Adapter
    Source/Magic/Adapter/Win//Utils.cpp
    Source/Magic/Adapter/Win//Mutex.cpp
    Source/Magic/Adapter/Linux/Utils.cpp
    Source/Magic/Adapter/Linux/Mutex.cpp
# NetWork
    Source/Magic/NetWork/IoPool.cpp
    Source/Magic/NetWork/Socket.cpp
    Source/Magic/NetWork/TcpClient.cpp
    Source/Magic/NetWork/TcpServer.cpp
    Source/Magic/NetWork/Http/Http.cpp
    Source/Magic/NetWork/Http/MultiPart.cpp
    Source/Magic/NetWork/Http/WebSocket.cpp
    Source/Magic/NetWork/Http/HttpParser.cpp
    Source/Magic/NetWork/Http/HttpSocket.cpp
    Source/Magic/NetWork/Http/HttpClient.cpp
    Source/Magic/NetWork/Http/HttpServer.cpp
    Source/Magic/NetWork/Http/HttpServlet.cpp
    Source/Magic/NetWork/Http/Utils/MultiPartParser.cpp
    Source/Magic/NetWork/Http/Utils/uri_parser.rl.cpp
    Source/Magic/NetWork/Http/Utils/http11_parser.rl.cpp
    Source/Magic/NetWork/Http/Utils/httpclient_parser.rl.cpp
)

if(MySql STREQUAL ON)
    set(SOURCES
        ${SOURCES}
        Source/Magic/DataBase/MySql.cpp
    )
    set(MAGIC_DEPEND_LIBRARY ${MAGIC_DEPEND_LIBRARY} mariadbclient)
endif()

set(BINARY Magic)
add_library(Magic ${SOURCES})
target_link_libraries(Magic ${MAGIC_DEPEND_LIBRARY})

if(Sanitize STREQUAL Thread)
    set(Analysis OFF)
elseif(Sanitize STREQUAL Address)
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