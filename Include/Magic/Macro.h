/*
 * @file: Macro.h
 * @Author: INotFound
 * @Date: 2020-03-13 22:34:16
 * @LastEditTime: 2020-03-14 00:09:40
 */
#pragma once
#include <assert.h>
#include "Log.h"
#include "Core.h"
#include "Thread.h"
#include "Config.h"
#include "Plugin.h"
#include "Adapter.h"

#define  MAGIC_LOG_LEVEL(logger,level)                                    \
                if(level >= logger->getLevel())                           \
                    Magic::LogWrap(                                       \
                        level,                                            \
                        Safe<Magic::LogEvent>(                            \
                            new Magic::LogEvent(                          \
                                __LINE__,                                 \
                                static_cast<uint64_t>(time(nullptr)),     \
                                1,                                        \
                                Magic::GetThreadId(),                     \
                                __FILE__,                                 \
                                logger->getLogName(),                     \
                                Magic::Thread::GetName())),               \
                        logger                                            \
                    ).get()

#define MAGIC_LOG_DEBUG(logger)       MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogDebug)

#define MAGIC_LOG_INFO(logger)        MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogInfo)

#define MAGIC_LOG_WARN(logger)        MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogWarn)

#define MAGIC_LOG_ERROR(logger)       MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogError)

#define MAGIC_LOG_FATAL(logger)       MAGIC_LOG_LEVEL(logger,Magic::LogLevel::LogFatal)

#define MAGIC_LOG_ROOT()              Magic::LoggerMgr::GetInstance()->getRoot()

#define MAGIC_LOG_NAME(name)          Magic::LoggerMgr::GetInstance()->getLogger(name)

#define MAGIC_INFO()                  MAGIC_LOG_LEVEL(Magic::LoggerMgr::GetInstance()->getRoot(),Magic::LogLevel::LogInfo)
#define MAGIC_WARN()                  MAGIC_LOG_LEVEL(Magic::LoggerMgr::GetInstance()->getRoot(),Magic::LogLevel::LogWarn)
#define MAGIC_DEBUG()                 MAGIC_LOG_LEVEL(Magic::LoggerMgr::GetInstance()->getRoot(),Magic::LogLevel::LogDebug)
#define MAGIC_ERROR()                 MAGIC_LOG_LEVEL(Magic::LoggerMgr::GetInstance()->getRoot(),Magic::LogLevel::LogError)
#define MAGIC_FATAL()                 MAGIC_LOG_LEVEL(Magic::LoggerMgr::GetInstance()->getRoot(),Magic::LogLevel::LogFatal)

#define MAGIC_ASSERT(exp,msg) \
    if(!(exp)) { \
        MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "ASSERTION: " << #exp \
            << "\n=>" << msg \
            << "\nBackTrace:\n" \
            << Magic::BackTraceToString(); \
        assert(exp); \
    }
