cmake_minimum_required(VERSION 3.0)
project(Magic)

set(SRC
    Magic.cpp
	Core/Log.cpp
	Core/Util.cpp
	Core/Thread.cpp
	Core/Config.cpp
	Core/Plugin.cpp
	Core/ByteArray.cpp
	Core/Adapter/Win32.cpp
	Core/Adapter/Linux.cpp
)



if(WIN32 AND NOT MINGW)
    message("~~~~~~~~~WIN32 Configuration~~~~~~~~")
    set(LIBS
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
     set(LIBS
        dl 
        pthread
     )
    add_library(Magic ${SRC})
    target_link_libraries(Magic ${LIBS})

endif()

#RapidJSON
include_directories(ThirdParty/RapidJSON/include)

add_library(TestLib MODULE Test/TestPlugin.cc)
add_executable(Test Test/Test.cc)
target_link_libraries(Test Magic)
