/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Trace.hpp
 * @Date           : 2023-07-03 18:30
 ******************************************************************************
 */
#pragma once

#include <mutex>
#include <chrono>
#include "Magic/Core/Core.hpp"

namespace Magic{
    class TraceTimer:public Noncopyable{
    public:
        ~TraceTimer();

        explicit TraceTimer(const StringView& funcName);

    private:
        std::string m_FunctionName;
        std::chrono::time_point<std::chrono::steady_clock> m_TimePoint;
    };

    class ITraceAppender:public Noncopyable{
    public:
        virtual ~ITraceAppender();

        virtual void complete() = 0;

        virtual void tracing(const StringView& funcName,uint64_t threadId,int64_t start,int64_t end) = 0;
    };

    /**
     * @brief Use Browser ://tracing
     */
    class ChromiumTraceAppender:public ITraceAppender{
    private:
        struct TraceData{
            int64_t m_End;
            int64_t m_Start;
            uint64_t m_ThreadId;
            std::string m_FuncName;
        };
    public:
        explicit ChromiumTraceAppender(const StringView& filePath);
        void complete() override;

        void tracing(const StringView& funcName,uint64_t threadId,int64_t start,int64_t end) override;

    private:
        std::mutex m_Mutex;
        std::string m_FilePath;
        std::vector<TraceData> m_Traces;
    };

    extern Safe<ITraceAppender> g_TraceAppender;
}

#ifndef TRACE
    #define MAGIC_TRACE_FUNCTION()
#else
    #define MAGIC_TRACE_FUNCTION() Magic::TraceTimer traceTimer_##funcName(__FUNCTION__)
#endif
