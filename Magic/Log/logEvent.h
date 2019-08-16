#pragma once
#include <string>
#include <sstream>

namespace Magic{
	
class LogEvent{
public:
	LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t fiberId,uint64_t threadId,const std::string& file,const std::string& LogName,const std::string& threadName);
    uint32_t getLine()                  const;
    uint64_t getTime()                  const;
    uint64_t getElapse()                const;
    uint64_t getFiberId()               const;
    uint64_t getThreadId()              const;
    std::stringstream& getStream();
    const std::string  getContent()     const;
    const std::string& getFile()        const;
    const std::string& getLogName()     const;
    const std::string& getThreadName()  const;
    
private:
    uint32_t m_Line       =0;
    uint64_t m_Time       =0;
    uint64_t m_Elapse     =0;
    uint64_t m_FiberId    =0;
    uint64_t m_ThreadId   =0;
    std::string m_File;
    std::string m_LogName;
    std::string m_ThreadName;
    std::stringstream m_StrStream;
};

}
