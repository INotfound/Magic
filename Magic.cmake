if(CMAKE_BUILD_TYPE STREQUAL Release)
    message("~~~~~~~~~~~~~~Release~~~~~~~~~~~~~~~")
    if(WIN32 AND NOT MINGW)
        message("~~~~~~~~~WIN32 Configuration~~~~~~~~")
    elseif(WIN32 AND MINGW)
        message("~~~~~~~~~MINGW Configuration~~~~~~~~")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    elseif(UNIX AND NOT ANDROID)
        message("~~~~~~~~~UNIX Configuration~~~~~~~~~")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()

else()
    message("~~~~~~~~~~~~~~~Debug~~~~~~~~~~~~~~~~")
    if(WIN32 AND NOT MINGW)
        message("~~~~~~~~~WIN32 Configuration~~~~~~~~")
        add_definitions(/DPERFORMANCE)
    elseif(WIN32 AND MINGW)
        message("~~~~~~~~~MINGW Configuration~~~~~~~~")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        add_definitions(-DPERFORMANCE)
    elseif(UNIX AND NOT ANDROID)
        message("~~~~~~~~~UNIX Configuration~~~~~~~~~")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        add_definitions(-DPERFORMANCE)
    endif()
endif()


if(WIN32)
    #WIN32 Library
    set(LIBS
        ws2_32
        mswsock
    )

elseif(UNIX)
    #UNIX Library
    set(LIBS
    )

endif()

if(MySql STREQUAL ON)
    set(LIBS ${LIBS} mariadbclient)
endif()

if(OpenSSL STREQUAL ON)
    set(LIBS ${LIBS} ssl crypto)
endif()

link_libraries(
    ${LIBS}
)
