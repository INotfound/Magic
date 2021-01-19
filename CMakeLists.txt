cmake_minimum_required(VERSION 3.0)

project(Magic)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
option(MySql "MYSQL module compilation options" OFF)
option(OpenSSl "OPENSSl module compilation options" OFF)

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

    #Source/Adapter/Win
    Source/Adapter/Linux/Utils.cpp
    Source/Adapter/Linux/Mutex.cpp
    
    Source/Utilty/CryptoUtils.cpp
    Source/NetWork/Http/Utils/url_parser.rl.cpp
    Source/NetWork/Http/Utils/http11_parser.rl.cpp
    Source/NetWork/Http/Utils/httpclient_parser.rl.cpp
)

if(MySql STRGREATER ON)
    add_definitions(MYSQL)
    set(SRC 
        ${SRC}
        Source/DataBase/MySql.cpp
    )
endif()

if(OpenSSl STRGREATER ON)
    add_definitions(OPENSSl)
endif()

add_library(Magic ${SRC})

#Tool
    add_executable(Gen Tool/Generator.cpp)
    add_executable(Create Tool/Create.cpp)

install(TARGETS Gen Create Magic
    RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/Bin
    ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/Lib
)

add_executable(Test Test/main.cpp)
target_link_libraries(Test Magic)

