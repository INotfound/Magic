cmake_minimum_required(VERSION 3.0)

project(Magic)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
option(MySql "MYSQL module compilation options" OFF)
option(OpenSSL "OPENSSL module compilation options" OFF)

include(Magic.cmake)
include_directories(
    Include
    ThirdParty/Asio/Include
    ThirdParty/RapidJSON/Include
)

set(SRC
    Source/Magic.cpp
    Source/Core/Timer.cpp
    Source/Core/Logger.cpp
    Source/Core/Config.cpp
    Source/Core/Thread.cpp
    Source/Core/TimingWheel.cpp

    Source/NetWork/IoPool.cpp
    Source/NetWork/Socket.cpp
    Source/NetWork/TcpServer.cpp

    Source/NetWork/Http/Http.cpp
    Source/NetWork/Http/HttpParser.cpp
    Source/NetWork/Http/HttpSocket.cpp
    Source/NetWork/Http/HttpServer.cpp
    Source/NetWork/Http/HttpServlet.cpp
    Source/NetWork/Http/HttpSession.cpp
    
    Source/Adapter/Win//Utils.cpp
    Source/Adapter/Win//Mutex.cpp
    
    Source/Adapter/Linux/Utils.cpp
    Source/Adapter/Linux/Mutex.cpp
    
    Source/Utilty/CryptoUtils.cpp
    Source/NetWork/Http/Utils/url_parser.rl.cpp
    Source/NetWork/Http/Utils/http11_parser.rl.cpp
    Source/NetWork/Http/Utils/httpclient_parser.rl.cpp
)

if(MySql STREQUAL ON)
    add_definitions(MYSQL)
    set(SRC 
        ${SRC}
        Source/DataBase/MySql.cpp
    )
endif()

if(OpenSSL STREQUAL ON)
    add_definitions(-DOPENSSL)
endif()

add_library(Magic ${SRC})

#Tool
set(BINARY
    Gen
    Magic
    Create
)

add_executable(Gen Tool/Generator.cpp)
add_executable(Create Tool/Create.cpp)

if(DEFINED MAGIC_CLANG_INCLUDE AND DEFINED MAGIC_CLANG_LIBRARY)
    set(BINARY ${BINARY} Analysis)
    link_directories(${MAGIC_CLANG_LIBRARY})
    include_directories(${MAGIC_CLANG_INCLUDE})
    add_executable(Analysis Tool/Analysis.cpp)
    target_link_libraries(Analysis libclang)
endif()

install(TARGETS ${BINARY}
    RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/Bin
    ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/Lib
)

add_executable(Test Test/main.cpp)
target_link_libraries(Test Magic)

