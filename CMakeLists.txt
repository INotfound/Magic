cmake_minimum_required(VERSION 3.0)
project(Magic)

include_directories(Include)
include_directories(Include/Magic)
set(SRC
    Source/Magic.cpp
	Source/Magic/Log.cpp
	Source/Magic/Util.cpp
	Source/Magic/Thread.cpp
	Source/Magic/Config.cpp
	Source/Magic/Plugin.cpp
    Source/Magic/IoPool.cpp
    Source/Magic/TcpServer.cpp
    Source/Magic/Http/Http.cpp
    Source/Magic/Http/HttpParser.cpp
    Source/Magic/Adapter/WinUtil.cpp
    Source/Magic/Adapter/WinMutex.cpp
    Source/Magic/Adapter/WinPlugin.cpp
    Source/Magic/Adapter/LinuxUtil.cpp
    Source/Magic/Adapter/LinuxMutex.cpp
	Source/Magic/Adapter/LinuxPlugin.cpp
    Source/Magic/Http/Util/http11_parser.rl.cpp
    Source/Magic/Http/Util/httpclient_parser.rl.cpp
)

#RapidJSON
include_directories(ThirdParty/RapidJSON/include)

#ASIO
include_directories(ThirdParty/Asio/asio/include)


if(WIN32 AND NOT MINGW)
    message("~~~~~~~~~WIN32 Configuration~~~~~~~~")
    set(LIBS
    )
    add_library(Magic ${SRC})

elseif(WIN32 AND MINGW)
    message("~~~~~~~~~MINGW Configuration~~~~~~~~")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    set(LIBS
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
        dl
    )
    add_library(Magic ${SRC})
    target_link_libraries(Magic ${LIBS})

endif()


add_library(TestLib MODULE Test/TestPlugin.cc)
add_executable(Test Test/Test.cc)
target_link_libraries(Test Magic)
