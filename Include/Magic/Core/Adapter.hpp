/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Adapter.hpp
 * @Date           : 2023-07-03 18:24
 ******************************************************************************
 */
#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <sys/stat.h>
    #include <io.h>
#undef DELETE
#endif

#if (defined(linux) || defined(__linux__))
    #include <dirent.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/time.h>
    #include <sys/stat.h>
    #include <sys/syscall.h>
#endif
