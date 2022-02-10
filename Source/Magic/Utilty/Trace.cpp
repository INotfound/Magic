#include <mutex>
#include "Magic/Utilty/Trace.h"
#include "Magic/Utilty/Thread.h"
namespace Magic{
    Safe<ITraceAppender> g_TraceAppender;

    TraceTimer::~TraceTimer(){
        auto timePoint = std::chrono::steady_clock::now();
        int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(timePoint).time_since_epoch().count();
        int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimePoint).time_since_epoch().count();
    #ifndef UTILTY
        if(g_TraceAppender){
            g_TraceAppender->tracing(m_FunctionName,0,start,end);
        }
    #else
        if(g_TraceAppender){
            g_TraceAppender->tracing(m_FunctionName,Magic::GetThreadId(),start,end);
        }
    #endif
    }

    TraceTimer::TraceTimer(const std::string& funcName)
        :m_FunctionName(funcName){
        m_TimePoint = std::chrono::steady_clock::now();
    }

    ITraceAppender::~ITraceAppender(){}

    ChromiumTraceAppender::~ChromiumTraceAppender() {
        this->complete();
    }

    ChromiumTraceAppender::ChromiumTraceAppender(const std::string &outFilePath)
        :m_UseSplit(false)
        ,m_FilePath(outFilePath){
        m_FileStream.open(outFilePath,std::ios_base::out|std::ios_base::trunc);
        if(m_FileStream.is_open()){
            m_FileStream << "{\"otherData\":{},\"traceEvents\":[";
            m_FileStream.flush();
        }
    }

    void ChromiumTraceAppender::complete() {
        if(!m_FileStream.is_open()){
            return;
        }
        m_FileStream << "]}";
        m_FileStream.flush();
        m_FileStream.close();
    }

    void ChromiumTraceAppender::tracing(const std::string &funcName,uint64_t threadId,int64_t start,int64_t end){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!m_FileStream.is_open()){
            return;
        }
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

