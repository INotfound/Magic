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
	Core/Log.cpp
	Core/Util.cpp
	Core/Thread.cpp
	Core/Adapter/Win32.cpp
	Core/Adapter/Linux.cpp
)

add_library(Magic ${SRC})