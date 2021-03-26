cmake_minimum_required(VERSION 3.0)

project(Magic)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MySql "MYSQL Module Compilation Options" OFF)
option(OpenSSL "OPENSSL Module Compilation Options" OFF)

option(Tool "TOOL Module Compilation Options" ON)
option(Utilty "UTILTY Module Compilation Options" ON)
option(NetWork "NETWORK Module Compilation Options" ON)

include(Magic.cmake)

include_directories(
    Include
    ThirdParty/Asio/Include
    ThirdParty/RapidJSON/Include
)

set(SOURCES
    Source/Magic/Core/Container.cpp
)

if(Utilty STREQUAL ON)
    set(SOURCES
        ${SOURCES}
        Source/Magic/Utilty/Timer.cpp
        Source/Magic/Utilty/Crypto.cpp
        Source/Magic/Utilty/Logger.cpp
        Source/Magic/Utilty/Config.cpp
        Source/Magic/Utilty/Thread.cpp
        Source/Magic/Utilty/TimingWheel.cpp
        
        Source/Magic/Adapter/Win//Utils.cpp
        Source/Magic/Adapter/Win//Mutex.cpp
        Source/Magic/Adapter/Linux/Utils.cpp
        Source/Magic/Adapter/Linux/Mutex.cpp
    )
endif()

if(NetWork STREQUAL ON)
    set(SOURCES
        ${SOURCES}
        Source/Magic/NetWork/IoPool.cpp
        Source/Magic/NetWork/Socket.cpp
        Source/Magic/NetWork/TcpServer.cpp
        Source/Magic/NetWork/Http/Http.cpp
        Source/Magic/NetWork/Http/HttpParser.cpp
        Source/Magic/NetWork/Http/HttpSocket.cpp
        Source/Magic/NetWork/Http/HttpServer.cpp
        Source/Magic/NetWork/Http/HttpServlet.cpp
        Source/Magic/NetWork/Http/HttpSession.cpp
        Source/Magic/NetWork/Http/Utils/url_parser.rl.cpp
        Source/Magic/NetWork/Http/Utils/http11_parser.rl.cpp
        Source/Magic/NetWork/Http/Utils/httpclient_parser.rl.cpp
    )
endif()

if(MySql STREQUAL ON)
    add_definitions(MYSQL)
    set(SOURCES 
        ${SOURCES}
        Source/Magic/DataBase/MySql.cpp
    )
endif()

if(OpenSSL STREQUAL ON)
    add_definitions(-DOPENSSL)
endif()

add_library(Magic ${SOURCES})

set(BINARY
    Magic
)

if(Sanitize STREQUAL Thread)
    set(Tool OFF)
elseif(Sanitize STREQUAL Address)
    set(Tool OFF)
endif()


if(Tool STREQUAL ON)

    set(BINARY
        Gen
        Create
        ${BINARY}
    )

    add_executable(Gen Tool/Generator.cpp)
    add_executable(Create Tool/Create.cpp)

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

add_executable(Test Test/main.cpp)
target_link_libraries(Test Magic)