
if(CMAKE_BUILD_TYPE STREQUAL Release)
    message("~~~~~~~~~~~~~~Release~~~~~~~~~~~~~~~")
    if(WIN32 AND NOT MINGW)
        message("~~~~~~~~~WIN32 Configuration~~~~~~~~")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ob")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ob")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
        add_definitions(/DPERFORMANCE)
    elseif(WIN32 AND MINGW)
        message("~~~~~~~~~MINGW Configuration~~~~~~~~")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        add_definitions(-DPERFORMANCE)
    endif()
endif()


if(WIN32 AND NOT MINGW)
    #WIN32 Library
    set(LIBS
        ws2_32
        mswsock
    )
    if(MySql STRGREATER ON)
        set(LIBS ${LIBS} mariadbclient)
    endif()
    if(OpenSSl STRGREATER ON)
        set(LIBS ${LIBS} ssl crypto)
    endif()
elseif(WIN32 AND MINGW)
    #MINGW Library
    set(LIBS
        ws2_32
        mswsock
    )
    if(MySql STRGREATER ON)
        set(LIBS ${LIBS} mariadbclient)
    endif()
    if(OpenSSl STRGREATER ON)
        set(LIBS ${LIBS} ssl crypto)
    endif()
elseif(UNIX AND NOT ANDROID)
    #UNIX Library
    set(LIBS
        dl
    )
    if(MySql STRGREATER ON)
        set(LIBS ${LIBS} mariadbclient)
    endif()
    if(OpenSSl STRGREATER ON)
        set(LIBS ${LIBS} ssl crypto)
    endif()
endif()

link_libraries(
    ${LIBS}
)

function(GENEX PATH NAME)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "" # list of names of mono-valued arguments
        "MODULE" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

    set(MODULEFILES)
    
    foreach(MODULEARG ${PARSED_ARGS_MODULE})
        set(MODULEFILES ${MODULEFILES} ${MODULEARG})
    endforeach(MODULEARG)

    execute_process(
        OUTPUT_VARIABLE MSG
        RESULT_VARIABLE ERROR
        COMMAND ${PATH}/Bin/Gen ${MODULEFILES} ${NAME}
    )

    if(ERROR STREQUAL 1)
        message(FATAL_ERROR "${MSG}")
    endif()
    
    include_directories(${CMAKE_BINARY_DIR})
endfunction(GENEX)