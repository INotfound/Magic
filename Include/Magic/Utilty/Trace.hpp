#pragma once

#include <mutex>
#include <chrono>
#include <fstream>
#include "Magic/Core/Core.hpp"

namespace Magic{
    class TraceTimer:public Noncopyable{
    public:
        ~TraceTimer();

        explicit TraceTimer(const Magic::StringView& funcName);

    private:
        std::string m_FunctionName;
        std::chrono::time_point<std::chrono::steady_clock> m_TimePoint;
    };

    class ITraceAppender:public Noncopyable{
    public:
        virtual ~ITraceAppender();

        virtual void complete() = 0;

        virtual void tracing(const Magic::StringView& funcName,uint64_t threadId,int64_t start,int64_t end) = 0;
    };

    /**
     * @brief Use Browser ://tracing
     */
    class ChromiumTraceAppender:public ITraceAppender{
    public:
        ~ChromiumTraceAppender() override;

        explicit ChromiumTraceAppender(const Magic::StringView& outFilePath);

        void complete() override;

        void tracing(const Magic::StringView& funcName,uint64_t threadId,int64_t start,int64_t end) override;

    private:
        bool m_UseSplit;
        std::mutex m_Mutex;
        std::string m_FilePath;
        std::ofstream m_FileStream;
    };

    extern Safe<ITraceAppender> g_TraceAppender;
}

#ifndef TRACE
#define MAGIC_TRACE_FUNCTION(funcName)
#else
#define MAGIC_TRACE_FUNCTION(funcName) Magic::TraceTimer traceTimer_##funcName(funcName)
#endif
