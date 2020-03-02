cmake_minimum_required(VERSION 3.0)
project(Magic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC
#   Util
    Source/Magic.cpp
	Source/Magic/Log.cpp
    Source/Magic/Gzip.cpp
    Source/Magic/Timer.cpp
    Source/Magic/Crypto.cpp
	Source/Magic/Thread.cpp
	Source/Magic/Config.cpp
	Source/Magic/Plugin.cpp
    Source/Magic/IoPool.cpp
#   Source/Magic/Socket.cpp
    Source/Magic/Uri.rl.cpp
    Source/Magic/TcpServer.cpp
    Source/Magic/TimingWheel.cpp
#   DataBase
    Source/Magic/DB/MySql.cpp
#   Http
    Source/Magic/Http/Http.cpp
    Source/Magic/Http/Session.cpp
    Source/Magic/Http/HttpFile.cpp
    Source/Magic/Http/MultiPart.cpp
    Source/Magic/Http/HttpCache.cpp
    Source/Magic/Http/HttpServer.cpp
    Source/Magic/Http/HttpParser.cpp
    Source/Magic/Http/HttpServlet.cpp
#   WebSocket
    Source/Magic/WebSocket/WebSocketServer.cpp
#   Adapter
    Source/Magic/Adapter/WinUtil.cpp
    Source/Magic/Adapter/WinMutex.cpp
    Source/Magic/Adapter/WinPlugin.cpp
    Source/Magic/Adapter/LinuxUtil.cpp
    Source/Magic/Adapter/LinuxMutex.cpp
	Source/Magic/Adapter/LinuxPlugin.cpp
    Source/Magic/Http/Util/http11_parser.rl.cpp
    Source/Magic/Http/Util/httpclient_parser.rl.cpp
)

include_directories(Include)
include_directories(Include/Magic)
include_directories(ThirdParty/Gzip)                #Gzip
include_directories(ThirdParty/RapidJSON/include)   #RapidJSON
include_directories(ThirdParty/Asio/asio/include)   #ASIO

if(WIN32 AND NOT MINGW)
    message("~~~~~~~~~WIN32 Configuration~~~~~~~~")
    
    #GZip
    add_definitions(-D Z_PREFIX)
    add_subdirectory(ThirdParty/Gzip)
    set(LIBS
        zlib
        mysqlclient
    )
    add_library(Magic ${SRC})
elseif(WIN32 AND MINGW)
    message("~~~~~~~~~MINGW Configuration~~~~~~~~")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

    #GZip
    add_definitions(-D Z_PREFIX)
    add_subdirectory(ThirdParty/Gzip)
    set(LIBS
        zlib
        mysqlclient
    )
    add_library(Magic ${SRC})
elseif(UNIX AND NOT ANDROID)
    message("~~~~~~~~~UNIX Configuration~~~~~~~~~")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(LIBS
        z
        dl
        ssl
        crypto
        mysqlclient
    )
    add_library(Magic ${SRC})
endif()

target_link_libraries(Magic ${LIBS})

add_library(TestLib MODULE Test/TestPlugin.cc)
add_executable(Test Test/Test.cc)
target_link_libraries(Test Magic)
