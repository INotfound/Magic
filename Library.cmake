########################################################################################
#   Include
set(OPENSSL_INCLUDE "C:\\Users\\magic\\Desktop\\openssl\\ssl\\include")
set(MYSQL_INCLUDE "C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\include")
#   Library
set(OPENSSL_LIBRARY "C:\\Users\\magic\\Desktop\\openssl\\ssl\\lib")
set(MYSQL_LIBRARY "C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\lib")
########################################################################################

if(WIN32 AND NOT MINGW)
    message("~~~~~~~~~WIN32 Configuration~~~~~~~~")

    #   Include
    include_directories(
        ${MYSQL_INCLUDE}
        ${OPENSSL_INCLUDE}
    )
    #   Library
    link_directories(
        ${MYSQL_LIBRARY}
        ${OPENSSL_LIBRARY}
    )

    #GZip
    add_subdirectory(ThirdParty/Gzip)

    #Library
    set(LIBS
        ssl
        mysql
        ws2_32
        crypto
        mswsock
    )
elseif(WIN32 AND MINGW)
    message("~~~~~~~~~MINGW Configuration~~~~~~~~")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    #   Include
    include_directories(
        ${MYSQL_INCLUDE}
        ${OPENSSL_INCLUDE}
    )
    #   Library
    link_directories(
        ${MYSQL_LIBRARY}
        ${OPENSSL_LIBRARY}
    )

    #Library
    set(LIBS
        ssl
        mysql
        ws2_32
        crypto
        mswsock
    )
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
        ssl
        crypto
        mysqlclient
    )
endif()

link_libraries(
    ${LIBS}
)