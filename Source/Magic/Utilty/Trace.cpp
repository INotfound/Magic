/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Trace.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include <mutex>
#include <random>
#include "Magic/Utilty/Trace.hpp"
#include "Magic/Utilty/Thread.hpp"
#include "Magic/Utilty/String.hpp"

namespace Magic{
    Safe<ITraceAppender> g_TraceAppender;

    TraceTimer::~TraceTimer(){
        auto timePoint = std::chrono::steady_clock::now();
        int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(timePoint).time_since_epoch().count();
        int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimePoint).time_since_epoch().count();

        if(g_TraceAppender){
            g_TraceAppender->tracing(m_FunctionName,Magic::GetThreadId(),start,end);
        }
    }

    TraceTimer::TraceTimer(const Magic::StringView& funcName)
        :m_FunctionName(funcName.data(),funcName.size()){
        m_TimePoint = std::chrono::steady_clock::now();
    }

    ITraceAppender::~ITraceAppender() = default;

    ChromiumTraceAppender::~ChromiumTraceAppender(){
        this->complete();
    }

    ChromiumTraceAppender::ChromiumTraceAppender(const Magic::StringView& outFilePath)
        :m_UseSplit(false)
        ,m_FilePath(outFilePath.data(),outFilePath.size()){
    }

    void ChromiumTraceAppender::complete(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!m_FileStream.is_open()){
            return;
        }
        m_FileStream << "]}";
        m_FileStream.flush();
        m_FileStream.close();
    }

    void ChromiumTraceAppender::tracing(const Magic::StringView& funcName,uint64_t threadId,int64_t start,int64_t end){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!m_FileStream.is_open()){
            std::string filePath = m_FilePath;
            filePath.append(".");
            std::uniform_int_distribution<uint16_t> uniformIntDistribution;
            std::default_random_engine randomEngine(std::chrono::steady_clock::now().time_since_epoch().count());
            filePath.append(Magic::AsString(uniformIntDistribution(randomEngine)));
            m_FileStream.open(filePath,std::ios_base::out | std::ios_base::trunc);
            if(m_FileStream.is_open()){
                m_FileStream << "{\"otherData\":{},\"traceEvents\":[";
                m_UseSplit = false;
            }
        }

        if(m_FileStream.is_open()){
            if(!m_UseSplit){
                m_UseSplit = true;
            }else{
                m_FileStream << ",";
            }
            m_FileStream << "{"
                         << "\"cat\":\"function\","
                         << "\"dur\":" << (end-start) << ","
                         << "\"name\":\"" << funcName << "\","
                         << "\"ph\":\"X\","
                         << "\"pid\":0,"
                         << "\"tid\":" << threadId << ","
                         << "\"ts\":" << start
                         << "}";
            m_FileStream.flush();
        }
    }
}

