project(Server)
cmake_minimum_required(VERSION 2.6)


if(WIN32 AND NOT MINGW)
    message("~~~~~~~~~WIN32~~~~~~~~")
elseif(WIN32 AND MINGW)
    message("~~~~~~~~~MINGW~~~~~~~~")
elseif(UNIX AND NOT ANDROID)
    message("~~~~~~~~~UNIX~~~~~~~~~")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
elseif(ANDROID)
	message("~~~~~~~~~ANDROID~~~~~~")
endif()

set(SRC
	Magic/Core/Log.cpp
	Magic/Core/Util.cpp
	Magic/Core/Thread.cpp
	Magic/Core/Adapter/Win32.cpp
	Magic/Core/Adapter/Linux.cpp
)

add_executable(Server main.cc ${SRC})
#target_link_libraries(Server ${SRC})
