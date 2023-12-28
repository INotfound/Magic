/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Trace.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include <mutex>
#include <random>
#include "Magic/Core/Stream.hpp"
#include "Magic/Utilty/Trace.hpp"
#include "Magic/Utilty/String.hpp"

namespace Magic{
    Safe<ITraceAppender> g_TraceAppender;

    TraceTimer::~TraceTimer(){
        auto timePoint = std::chrono::steady_clock::now();
        int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(timePoint).time_since_epoch().count();
        int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimePoint).time_since_epoch().count();

        if(g_TraceAppender){
            g_TraceAppender->tracing(m_FunctionName,GetThreadId(),start,end);
        }
    }

    TraceTimer::TraceTimer(const StringView& funcName)
        :m_FunctionName(funcName.data(),funcName.size()){
        m_TimePoint = std::chrono::steady_clock::now();
    }

    ITraceAppender::~ITraceAppender() = default;

    ChromiumTraceAppender::ChromiumTraceAppender(const StringView& filePath)
        :m_FilePath(filePath.data(),filePath.size()){
    }

    void ChromiumTraceAppender::complete(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        FileStream fileStream(m_FilePath);
        if(fileStream.open(FileStream::OpenMode::Write)){
            fileStream.write("{\"otherData\":{},\"traceEvents\":[");
            for(std::vector<TraceData>::size_type i = 0;i < m_Traces.size();i++){
                std::stringstream object;
                if(i > 0){
                    object << ",";
                }
                const auto& value = m_Traces.at(i);
                object << "{"
                       << "\"cat\":\"function\","
                       << "\"dur\":" << (value.m_End-value.m_Start) << ","
                       << "\"name\":\"" << value.m_FuncName << "\","
                       << "\"ph\":\"X\","
                       << "\"pid\":0,"
                       << "\"tid\":" << value.m_ThreadId << ","
                       << "\"ts\":" << value.m_Start
                       << "}";
                fileStream.write(object.str());
            }
            fileStream.write("]}");
        }
    }

    void ChromiumTraceAppender::tracing(const StringView& funcName,uint64_t threadId,int64_t start,int64_t end){
        std::lock_guard<std::mutex> locker(m_Mutex);
        TraceData data;
        data.m_End = end;
        data.m_Start = start;
        data.m_ThreadId = threadId;
        data.m_FuncName = std::string(funcName.data(),funcName.size());
        m_Traces.push_back(data);
    }
}

